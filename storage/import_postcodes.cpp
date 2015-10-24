#include "import_postcodes.h"

#include <boost/algorithm/string.hpp>
#include <fstream>
#include <stdlib.h>
#include "../app/defines.h"
#include "../app/global.h"
#include "dbo/postcode.h"

#define FILEBUFFER_SIZE (10*1024)

#define EARTH_RADIUS (3958.75)
#define METER_CONVERSION (1609.00)


using namespace modellbasen;

bool PostCodesImporter::Import(const std::string& filename, WebApplication* app, Wt::WProgressBar* progressbar, Wt::WString& import_status)
{
	std::ifstream file(filename, std::ios::in|std::ios::binary|std::ios::ate); //ios::ate to seek to end of file (to easily find the file size)
	if (!file.is_open())
	{
		import_status = Wt::WString::tr("System.FileOpenFailed");
		return false;
	}
	
	size_t filesize = file.tellg();
	file.seekg(0, std::ios::beg);

	size_t remaining_bytes_of_file = filesize;
	size_t filebuffer_length = 0;
	size_t filebuffer_pos = 0;
	char* filebuffer = new char[FILEBUFFER_SIZE];
	if (!filebuffer)
	{
		import_status = Wt::WString::tr("System.OutOfMemory");
		file.close();
		return false;
	}

	Poco::Data::Session* session_in_transaction;
	if (!DB.CreateSession(session_in_transaction))
	{
		delete filebuffer;
		file.close();
		import_status = Wt::WString::tr("Database.FailedToCreateSession");
		return false;
	}

	progressbar->setRange(0, filesize);
	progressbar->setValue(0);
	app->processEvents();

	int line = 0;
	std::vector<std::string> values;
	while (PostCodesImporter::MORE_CONTENT == ParseCSVLine(file, filebuffer, remaining_bytes_of_file, filebuffer_length, filebuffer_pos, values))
	{
		line++;
		if (1==line)
		{
			if (EQUAL != values[0].compare("POSTNR"))
			{
				delete[] filebuffer;
				file.close();
				DB.ReleaseSession(session_in_transaction, PocoGlue::ROLLBACK);
				import_status = Wt::WString::tr("Import.UnexpectedFileFormat");
				return false;
			}
			continue; //Skip first line
		}

		if (EQUAL==values[3].compare("Postboksar") ||
		    std::string::npos!=values[1].find("ikkje i bruk"))
		{
			continue; //Skip postcodes where people do not live
		}

		double latitude = strtod(values[9].c_str(), NULL);
		double longitude = strtod(values[10].c_str(), NULL);
		if (!StorePostCode(session_in_transaction, values[0],
		                                           values[1],
		                                           latitude,
		                                           longitude))
		{
			delete[] filebuffer;
			file.close();
			DB.ReleaseSession(session_in_transaction, PocoGlue::ROLLBACK);
			import_status = Wt::WString::tr("Import.FileParseErrorLine").arg(line);
			return true;
		}

		if (0 == (line%100))
		{
			progressbar->setValue(filesize - remaining_bytes_of_file);
			app->processEvents();
		}
	}
	delete[] filebuffer;
	file.close();

	if (!CalculatePostCodeDistances(session_in_transaction, app, progressbar))
	{
		DB.ReleaseSession(session_in_transaction, PocoGlue::ROLLBACK);
		import_status = Wt::WString::tr("Import.FailedCalculatingDistances");
		return false;
	}
	
	DB.ReleaseSession(session_in_transaction, PocoGlue::COMMIT);
	import_status = Wt::WString::tr("Import.Success");
	return true;
}

PostCodesImporter::ImportStatus PostCodesImporter::ParseCSVLine(std::ifstream& file,
                                                       char* filebuffer, size_t& remaining_bytes_of_file, size_t& filebuffer_length, size_t& filebuffer_pos,
                                                       std::vector<std::string>& values)
{
	values.clear();

	uint8_t remaining_utf8_bytes = 0;
	std::string value;
	uint8_t ch;
	while (!(0==remaining_bytes_of_file && filebuffer_pos==filebuffer_length))
	{
		if (filebuffer_pos >= filebuffer_length)
		{
			FillFilebuffer(file, filebuffer, remaining_bytes_of_file, filebuffer_length, filebuffer_pos);
		}

		if (0<remaining_utf8_bytes)
		{
			value += filebuffer[filebuffer_pos++];
			remaining_utf8_bytes--;
			continue;
		}

		ch = filebuffer[filebuffer_pos++];
		if ('\r'==ch)
		{
			continue;
		}
		else if ('\n'==ch)
		{
			return PostCodesImporter::MORE_CONTENT;
		}
		else if ('\t'==ch)
		{
			boost::trim(value);
			values.push_back(value);
			value.clear();
		}
		else if (128 > ch)
		{
			value += (char)ch;
		}
		else
		{
			if (0xC0 == (ch&0xE0))
			{
				remaining_utf8_bytes = 2;
			}
			else if (0xE0 == (ch&0xF0))
			{
				remaining_utf8_bytes = 3;
			}
			else if (0xF0 == (ch&0xF8))
			{
				remaining_utf8_bytes = 4;
			}
			else //Invalid UTF-8
			{
				value += '?';
				continue;
			}

			value += (char)ch;
			remaining_utf8_bytes--;
		}
	}

	return PostCodesImporter::NO_MORE_CONTENT;
}

void PostCodesImporter::FillFilebuffer(std::ifstream& file, char* filebuffer, size_t& remaining_bytes_of_file, size_t& filebuffer_length, size_t& filebuffer_pos) const
{
	if (filebuffer_pos < filebuffer_length)
	{
		memmove(filebuffer, filebuffer+filebuffer_pos, filebuffer_length-filebuffer_pos);
	}
	filebuffer_length -= filebuffer_pos;
	filebuffer_pos = 0;

	size_t read_bytes = MIN(remaining_bytes_of_file, FILEBUFFER_SIZE);
	read_bytes -= filebuffer_length;
	file.read(filebuffer+filebuffer_length, read_bytes);
	filebuffer_length += read_bytes;
	remaining_bytes_of_file -= read_bytes;
}

bool PostCodesImporter::StorePostCode(Poco::Data::Session* session_in_transaction,
                                      const std::string& postcode, const std::string& place, double latitude, double longitude)
{
	*session_in_transaction << "INSERT INTO postcode (postcode, place, latitude, longitude) "\
	                            "VALUE (?, ?, ?, ?) "\
	                            "ON DUPLICATE KEY UPDATE place=?, latitude=?, longitude=?;",
		Poco::Data::use(postcode), Poco::Data::use(place), Poco::Data::use(latitude), Poco::Data::use(longitude),
		Poco::Data::use(place), Poco::Data::use(latitude), Poco::Data::use(longitude),
		Poco::Data::now;
	
	return true;
}

bool PostCodesImporter::CalculatePostCodeDistances(Poco::Data::Session* session_in_transaction, WebApplication* app, Wt::WProgressBar* progressbar)
{
	std::list<PostCode> postcodes;
	*session_in_transaction << "SELECT postcode, place, latitude, longitude FROM postcode;", Poco::Data::into(postcodes), Poco::Data::now;

	size_t total_distances = (postcodes.size() * (postcodes.size()-1)) / 2;
	progressbar->setRange(0, total_distances);
	progressbar->setValue(0);
	app->processEvents();

	std::string source_postcode;
	std::string destination_postcode;
	double distance;
	
	Poco::Data::Statement insert_statement(*session_in_transaction);
	insert_statement << "INSERT INTO postcode_distance (source, destination, distance) "\
		                   "VALUE (?, ?, ?) "\
		                   "ON DUPLICATE KEY UPDATE distance=?;",
			Poco::Data::use(source_postcode), Poco::Data::use(destination_postcode), Poco::Data::use(distance), Poco::Data::use(distance);

	size_t processed_distances = 0;
	while (1 < postcodes.size())
	{
		const PostCode& source = postcodes.front();
		source_postcode = source.GetPostCode();

		for(std::list<PostCode>::iterator it = ++(postcodes.begin()); it!=postcodes.end(); ++it)
		{
			const PostCode& destination = *it;
			destination_postcode = destination.GetPostCode(); 
			distance = DistanceBetweenPoints(source.GetLatitude(), source.GetLongitude(), destination.GetLatitude(), destination.GetLongitude());

			insert_statement.execute();

			processed_distances++;
			if (0 == (processed_distances%10000))
			{
				progressbar->setValue(processed_distances);
				app->processEvents();
			}
		}

		postcodes.pop_front();
	}
	return true;
}

double PostCodesImporter::DistanceBetweenPoints(double lat1, double lng1, double lat2, double lng2) const
{
  double delta_latitude = DEG2RAD(lat2-lat1);
  double delta_longitude = DEG2RAD(lng2-lng1);
  double a = sin(delta_latitude/2) * sin(delta_latitude/2) + 
             cos(DEG2RAD(lat1)) * cos(DEG2RAD(lat2)) * 
             sin(delta_longitude/2) * sin(delta_longitude/2);
  return 2 * atan2(sqrt(a), sqrt(1-a)) * EARTH_RADIUS * METER_CONVERSION;
}
