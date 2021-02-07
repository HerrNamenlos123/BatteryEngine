
#include "Battery/pch.h"

/*
#include "Battery/NetUtils.h"
#include "Battery/FileUtils.h"

#include <string>
#include <sstream>
#include <iostream>
#include <thread>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Infos.hpp>
#include <curlpp/Options.hpp>

namespace Battery {
	namespace NetUtils {

		HTTPRequest GetHTTPRequest(const std::string& url, const std::string& certificates, const std::string& userAgent, long timeout) {

			using namespace curlpp::options;

			try {
				curlpp::Cleanup cleanup;
				curlpp::Easy request;

				// Set the URL.
				request.setOpt<Url>(url);
				request.setOpt<CaInfo>(certificates);
				request.setOpt<FollowLocation>(1L);
				request.setOpt<UserAgent>(userAgent);
				request.setOpt<Timeout>(timeout);

				std::ostringstream response;
				request.setOpt(new curlpp::options::WriteStream(&response));

				request.perform();

				long status = curlpp::infos::ResponseCode::get(request);

				HTTPRequest r;
				r.url = url;
				r.responseCode = status;
				r.response = response.str();

				if (status != 200) {
					std::cerr << "Server responded with code " << status << std::endl;

					r.valid = false;
					return r;
				}

				r.valid = true;
				return r;
			}

			catch (curlpp::RuntimeError& e)
			{
				std::cerr << "[NetUtils]: " << e.what() << std::endl;

				HTTPRequest r;
				r.url = url;
				r.responseCode = 0;
				r.response = "";
				r.valid = false;
				return r;
			}

			catch (curlpp::LogicError& e)
			{
				std::cerr << "[NetUtils]: " << e.what() << std::endl;

				HTTPRequest r;
				r.url = url;
				r.responseCode = 0;
				r.response = "";
				r.valid = false;
				return r;
			}
		}

		/*size_t GetSizeOfDownloadFile(const std::string& url, const std::string& certificates, const std::string& userAgent, long timeout) {

			using namespace curlpp::options;

			try {
				curlpp::Cleanup cleanup;
				curlpp::Easy request;

				// Set the URL.
				request.setOpt<Url>(url);
				request.setOpt<CaInfo>(certificates);
				request.setOpt<UserAgent>(userAgent);
				request.setOpt<Timeout>(timeout);
				request.setOpt<FollowLocation>(1L);
				request.setOpt<Verbose>(1L);
				request.setOpt<Header>(1L);
				request.setOpt<NoBody>(1L);

				request.perform();

				long status = curlpp::infos::ResponseCode::get(request);
				size_t filesize = curlpp::infos::ContentLengthDownload::get(request);

				if (status != 200) {
					std::cerr << "Server responded with code " << status << std::endl;
					return -1;
				}

				if (filesize == 0)
					return -1;

				return filesize;
			}

			catch (curlpp::RuntimeError& e)
			{
				std::cerr << "[NetUtils]: " << e.what() << std::endl;
				return -1;
			}

			catch (curlpp::LogicError& e)
			{
				std::cerr << "[NetUtils]: " << e.what() << std::endl;
				return -1;
			}

			return -1;
		}*/
		
		/*bool FileDownloader::download(const std::string& url, const std::string& outputFile, const std::string& caFile) {
			CURL* curl = nullptr;
			FILE* fp = nullptr;
			CURLcode res;

			// Initialize curl context
			curl = curl_easy_init();
			if (!curl)
				return false;

			// Try to download file
			errno_t error = fopen_s(&fp, outputFile.c_str(), "wb");

			if (error != 0) {
				curl_easy_cleanup(curl);
				return false;
			}

			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _writeData);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
			curl_easy_setopt(curl, CURLOPT_CAINFO, caFile.c_str());
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			res = curl_easy_perform(curl);
			fclose(fp);
			curl_easy_cleanup(curl);

			std::cout << std::endl;

			if (res != 0) {		// Downloader failed, delete file
				FileUtils::RemoveFile(outputFile);
				return false;
			}
		}

		size_t _writeData(void* ptr, size_t size, size_t nmemb, FILE* stream) {
			std::cout << "Size: " << size << std::endl;
			std::cout << "Count: " << nmemb << std::endl;
			size_t written = fwrite(ptr, size, nmemb, stream);
			return written;
		}

		bool DownloadFile(const std::string& url, const std::string& outputFile, const std::string& caFile) {

			FileDownloader downloader;

			std::thread t(downloader.download, url, outputFile, caFile);
			bool s = downloader.download(url, outputFile, caFile);

			//std::cout << "Result: " << res << std::endl;

			return s;
		}

	}
}

*/