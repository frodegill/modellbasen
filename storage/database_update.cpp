#include "database_update.h"
#include "../app/global.h"


using namespace modellbasen;

static DatabaseUpdateItem update_items[] = {
//Index should be incremented every time you want to commit transaction.
//(CREATE TABLE automatically commits, and items preceding CREATE TABLE should thus always increment index in case of the need for a rollback)

//"CREATE TABLE metadata" is done in DatabaseUpdate::GetDatabaseVersion

{ 1, "INSERT INTO `modellbasen`.`datatype` (`id`, `description`) VALUE (1, 'integer');"},
{ 1, "INSERT INTO `modellbasen`.`datatype` (`id`, `description`) VALUE (2, 'string');"},
{ 1, "INSERT INTO `modellbasen`.`datatype` (`id`, `description`) VALUE (3, 'datetime');"},
{ 1, "INSERT INTO `modellbasen`.`datatype` (`id`, `description`) VALUE (4, 'boolean (true if present)');"},
{ 1, "INSERT INTO `modellbasen`.`datatype` (`id`, `description`) VALUE (5, 'location');"},
{ 1, "INSERT INTO `modellbasen`.`datatype` (`id`, `description`) VALUE (6, 'singleselect');"},
{ 1, "INSERT INTO `modellbasen`.`datatype` (`id`, `description`) VALUE (7, 'multiselect');"},
{ 1, "INSERT INTO `modellbasen`.`datatype` (`id`, `description`) VALUE (8, 'height_range');"},
{ 1, "INSERT INTO `modellbasen`.`datatype` (`id`, `description`) VALUE (9, 'day_range');"},
{ 1, "INSERT INTO `modellbasen`.`datatype` (`id`, `description`) VALUE (10, 'age_range');"},
{ 1, "INSERT INTO `modellbasen`.`datatype` (`id`, `description`) VALUE (11, 'distance');"},

{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (1, 'tag.administrator', 4, 4, NULL, true);"},

{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (2, 'tag.name', 2, 2, NULL, false);"},

{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (3, 'tag.sirname', 2, 2, NULL, false);"},

{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (4, 'tag.sex', 6, 6, NULL, false);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (1, 'tagvalue.female', 1, 4);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (2, 'tagvalue.male', 2, 4);"},

{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (5, 'tag.birthday', 3, 10, NULL, false);"},

{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (6, 'tag.registeredtime', 3, 9, NULL, true);"},

{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (7, 'tag.lastvisitedtime', 3, 9, NULL, true);"},

{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (8, 'tag.postcode', 5, 11, NULL, false);"},

{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (9, 'tag.confirmedby', 1, 2, NULL, true);"},

{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (10, 'tag.photographer', 4, 4, NULL, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (11, 'tag.photographer.experience', 7, 7, 10, false);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (3, 'tagvalue.none', 1, 11);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (4, 'tagvalue.some', 2, 11);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (5, 'tagvalue.a_lot', 3, 11);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (6, 'tagvalue.professional', 4, 11);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (12, 'tag.photographer.ownstudio', 4, 4, 10, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (13, 'tag.photographer.preferredlocation', 7, 7, 10, false);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (7, 'tagvalue.studio', 1, 13);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (8, 'tagvalue.nature', 2, 13);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (9, 'tagvalue.ceremony', 3, 13);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (14, 'tag.photographer.tfp', 4, 4, 10, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (15, 'tag.photographer.companyname', 2, 2, 10, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (16, 'tag.photographer.companyaddress', 2, 2, 10, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (17, 'tag.photographer.webpage', 2, 2, 10, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (18, 'tag.photographer.publicemail', 2, 2, 10, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (19, 'tag.photographer.phone', 2, 2, 10, false);"},

{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (20, 'tag.model', 4, 4, NULL, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (21, 'tag.model.height', 1, 8, 20, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (22, 'tag.model.haircolour', 7, 7, 20, false);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (10, 'tagvalue.lightblonde', 1, 22);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (11, 'tagvalue.darkblonde', 2, 22);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (12, 'tagvalue.black', 3, 22);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (13, 'tagvalue.red', 4, 22);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (23, 'tag.model.hairlength', 7, 7, 20, false);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (14, 'tagvalue.short', 1, 23);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (15, 'tagvalue.medium', 2, 23);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (16, 'tagvalue.long', 3, 23);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (24, 'tag.model.tattoo', 4, 4, 20, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (25, 'tag.model.piercing', 4, 4, 20, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (26, 'tag.model.experience', 7, 7, 20, false);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (17, 'tagvalue.none', 1, 26);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (18, 'tagvalue.some', 2, 26);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (19, 'tagvalue.a_lot', 3, 26);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (20, 'tagvalue.professional', 4, 26);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (27, 'tag.model.tfp', 7, 7, 20, false);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (21, 'tagvalue.advertise', 1, 27);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (22, 'tagvalue.location', 2, 27);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (23, 'tagvalue.studio', 3, 27);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (24, 'tagvalue.lingerie', 4, 27);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (25, 'tagvalue.nude_art', 5, 27);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (28, 'tag.model.paid', 7, 7, 20, false);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (26, 'tagvalue.advertise', 1, 28);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (27, 'tagvalue.location', 2, 28);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (28, 'tagvalue.studio', 3, 28);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (29, 'tagvalue.lingerie', 4, 28);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (30, 'tagvalue.nude_art', 5, 28);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (29, 'tag.model.companyname', 2, 2, 20, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (30, 'tag.model.companyaddress', 2, 2, 20, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (31, 'tag.model.webpage', 2, 2, 20, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (32, 'tag.model.publicemail', 2, 2, 20, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (33, 'tag.model.phone', 2, 2, 20, false);"},

{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (34, 'tag.mua', 4, 4, NULL, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (35, 'tag.mua.experience', 7, 7, 34, false);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (31, 'tagvalue.none', 1, 35);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (32, 'tagvalue.some', 2, 35);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (33, 'tagvalue.a_lot', 3, 35);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (34, 'tagvalue.professional', 4, 35);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (36, 'tag.mua.tfp', 4, 4, 34, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (37, 'tag.mua.companyname', 2, 2, 34, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (38, 'tag.mua.companyaddress', 2, 2, 34, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (39, 'tag.mua.webpage', 2, 2, 34, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (40, 'tag.mua.publicemail', 2, 2, 34, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (41, 'tag.mua.phone', 2, 2, 34, false);"},

{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (42, 'tag.hairdresser', 4, 4, NULL, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (43, 'tag.hairdresser.experience', 7, 7, 42, false);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (35, 'tagvalue.none', 1, 43);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (36, 'tagvalue.some', 2, 43);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (37, 'tagvalue.a_lot', 3, 43);"},
{ 1, "INSERT INTO `modellbasen`.`tagvalue` (`id`, `value`, `pos`, `tag`) VALUE (38, 'tagvalue.professional', 4, 43);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (44, 'tag.hairdresser.tfp', 4, 4, 42, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (45, 'tag.hairdresser.companyname', 2, 2, 42, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (46, 'tag.hairdresser.companyaddress', 2, 2, 42, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (47, 'tag.hairdresser.webpage', 2, 2, 42, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (48, 'tag.hairdresser.publicemail', 2, 2, 42, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (49, 'tag.hairdresser.phone', 2, 2, 42, false);"},

{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (50, 'tag.agency', 4, 4, NULL, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (51, 'tag.agency.companyname', 2, 2, 50, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (52, 'tag.agency.companyaddress', 2, 2, 50, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (53, 'tag.agency.webpage', 2, 2, 50, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (54, 'tag.agency.publicemail', 2, 2, 50, false);"},
{ 1, "INSERT INTO `modellbasen`.`tag` (`id`, `name`, `insert_datatype`, `query_datatype`, `parent`, `query_only`) "\
	   "VALUE (55, 'tag.agency.phone', 2, 2, 50, false);"},

{ 1, "INSERT INTO `modellbasen`.`user` (`id`, `username`, `bcrypt_password_hash`, `email`, `profile_picture`) "\
	   "VALUE (1, 'admin', '$2y$10$ZU7iXUvqWkDxXU3/OSD/OOKp5SSzWfrR12LajHsezpAKpDyIBrvyC', 'admin@localhost', NULL);"}, //username:admin,password:admin
{ 1, "INSERT INTO `modellbasen`.`taginstance` (`id`, `stringvalue`, `intvalue`, `timevalue`, `tag`, `owner`, `eventparticipant`) "\
	   "VALUE (1, NULL, NULL, NULL, 1, 1, NULL);"},

{ 0, ""}
};

/*****************************************************************/

DatabaseUpdate::DatabaseUpdate()
{
}

DatabaseUpdate::~DatabaseUpdate()
{
}

bool DatabaseUpdate::GetDatabaseVersion(Poco::Data::Session* session_in_transaction, uint32_t& db_version) const
{
	db_version = 0;

	*session_in_transaction << "CREATE TABLE IF NOT EXISTS metadata "\
	                           "(id CHAR(50) CHARACTER SET utf8 NOT NULL,"\
	                           " value VARCHAR(4096) CHARACTER SET utf8,"\
	                           " PRIMARY KEY metadata_pk1 (id))"\
	                           " engine = InnoDB", Poco::Data::Keywords::now;

	uint16_t count;
	*session_in_transaction << "SELECT COUNT(*) FROM metadata WHERE id='db_version'",
			Poco::Data::Keywords::into(count,(const uint16_t)0), Poco::Data::Keywords::now;

	if (0==count)
	{
		*session_in_transaction << "INSERT INTO metadata (id, value) VALUE ('db_version',0);", Poco::Data::Keywords::now;

		if (!PocoGlue::CommitTransaction(session_in_transaction)) //Only CREATE TABLE, INSERT will get here
			return false;
	}

	*session_in_transaction << "SELECT value FROM metadata WHERE id='db_version'",
			Poco::Data::Keywords::into(db_version,(const uint32_t)0), Poco::Data::Keywords::now;

	return true;
}

bool DatabaseUpdate::UpdateIfNeeded(Poco::Data::Session* session_in_transaction)
{
	uint32_t db_version;
	if (!GetDatabaseVersion(session_in_transaction, db_version))
		return false;

	uint32_t max_version = db_version;
	size_t index = 0;
	while (0 != update_items[index].m_db_version)
	{
		if (db_version<update_items[index].m_db_version)
		{
			if (max_version<update_items[index].m_db_version)
				max_version = update_items[index].m_db_version;

			try
			{
				*session_in_transaction << update_items[index].m_update_query, Poco::Data::Keywords::now;

				if (update_items[index].m_db_version != update_items[index+1].m_db_version)
				{
					*session_in_transaction << "UPDATE metadata "\
					                           "SET value = ? "\
					                           "WHERE id='db_version'",
						Poco::Data::Keywords::use(max_version), Poco::Data::Keywords::now;

					if (!PocoGlue::CommitTransaction(session_in_transaction))
						return false;
				}
			}
			catch (Poco::Exception e)
			{
				::Log("error") << e.displayText();
				return false;
			}
			catch (...)
			{
				::Log("error") << "Got generic exception while updating database";
				return false;
			}
		}
		index++;
	}

	return true;
}
