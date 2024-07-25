#define DUCKDB_EXTENSION_MAIN

#include "quack3_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/scalar_function.hpp"
#include "duckdb/main/extension_util.hpp"
#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>
#include "duckdb/main/database_manager.hpp"
// OpenSSL linked through vcpkg
#include <openssl/opensslv.h>

namespace duckdb {

inline void QuackScalarFun3(DataChunk &args, ExpressionState &state, Vector &result) {
    auto &name_vector = args.data[0];
    UnaryExecutor::Execute<string_t, string_t>(
	    name_vector, result, args.size(),
	    [&](string_t name) {
			return StringVector::AddString(result, "Quack3 "+name.GetString()+" 🐥");;
        });
}

inline void QuackOpenSSLVersionScalarFun3(DataChunk &args, ExpressionState &state, Vector &result) {
    auto &name_vector = args.data[0];
    UnaryExecutor::Execute<string_t, string_t>(
	    name_vector, result, args.size(),
	    [&](string_t name) {
			return StringVector::AddString(result, "Quack3 " + name.GetString() +
                                                     ", my linked OpenSSL version is " +
                                                     OPENSSL_VERSION_TEXT );;
        });
}

static void LoadInternal3(DatabaseInstance &instance) {
    // Register a scalar function
    auto quack3_scalar_function = ScalarFunction("quack3", {LogicalType::VARCHAR}, LogicalType::VARCHAR, QuackScalarFun3);
    ExtensionUtil::RegisterFunction(instance, quack3_scalar_function);

    // Register another scalar function
    auto quack3_openssl_version_scalar_function = ScalarFunction("quack3_openssl_version", {LogicalType::VARCHAR},
                                                LogicalType::VARCHAR, QuackOpenSSLVersionScalarFun3);
    ExtensionUtil::RegisterFunction(instance, quack3_openssl_version_scalar_function);
}

void Quack3Extension::Load(DuckDB &db) {
	LoadInternal3(*db.instance);
}
std::string Quack3Extension::Name() {
	return "quack3";
}

std::string Quack3Extension::Version() const {
#ifdef EXT_VERSION_QUACK3
	return EXT_VERSION_QUACK3;
#else
	return "";
#endif
}

} // namespace duckdb

extern "C" {

DUCKDB_EXTENSION_API void quack3_init(duckdb::DatabaseInstance &db) {
/*	try {
            // Access the catalog
            auto &catalog = db.GetDatabaseManager().GetSystemCatalog();
            std::cout << "Successfully accessed system catalog" << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Exception in initialize: " << e.what() << std::endl;
            throw;
        }*/
    	duckdb::DuckDB db_wrapper(db);
    	db_wrapper.LoadExtension<duckdb::Quack3Extension>();
}

DUCKDB_EXTENSION_API const char *quack3_version() {
	return duckdb::DuckDB::LibraryVersion();
}
}

#ifndef DUCKDB_EXTENSION_MAIN
#error DUCKDB_EXTENSION_MAIN not defined
#endif
