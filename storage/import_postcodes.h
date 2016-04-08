#ifndef _IMPORT_POSTCODES_H_
#define _IMPORT_POSTCODES_H_

#include <string>
#include <vector>
#include <Wt/WProgressBar>
#include "poco_glue.h"
#include "../app/application.h"
#include "../singleton/db.h"


namespace modellbasen
{

class PostCodesImporter {
private:
	enum ImportStatus
	{
		MORE_CONTENT,
		NO_MORE_CONTENT
	};

public:
	bool Import(Poco::Data::Session* session_in_transaction, const std::string& filename,
	            WebApplication* app, Wt::WProgressBar* progressbar, Wt::WString& import_status);

private:
	ImportStatus ParseCSVLine(std::ifstream& file,
	                          char* filebuffer, size_t& remaining_bytes_of_file, size_t& filebuffer_length, size_t& filebuffer_pos,
	                          std::vector<std::string>& values);
	void FillFilebuffer(std::ifstream& file, char* filebuffer, size_t& remaining_bytes_of_file, size_t& filebuffer_length, size_t& filebuffer_pos) const;

	bool StorePostCode(Poco::Data::Session* session_in_transaction,
										 const std::string& postcode, const std::string& place, double latitude, double longitude);
	bool CalculatePostCodeDistances(Poco::Data::Session* session_in_transaction, WebApplication* app, Wt::WProgressBar* progressbar);

	double DistanceBetweenPoints(double lat1, double lng1, double lat2, double lng2) const; 
};

} // namespace modellbasen


#endif // _IMPORT_POSTCODES_H_
