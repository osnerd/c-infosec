
# **SessionSkid**

A C++ utility to gather system information, local IPs, and send notifications to a specified Telegram bot. This is especially useful for keeping track of system status in real-time. Upon execution, it collects system data and sends the details via a Telegram message.

---

## **Features**
- **System Information**: Retrieves information such as CPU architecture and memory usage.
- **IP Address**: Fetches both IPv4 and IPv6 addresses.
- **Telegram Notifications**: Sends gathered information to a Telegram bot.
- **Cross-Platform**: Works on both **Windows** and **Unix-like** systems.

---

## **How It Works**

The `InfoSec()` function performs the following steps:
1. Retrieves system information (CPU architecture, memory, etc.).
2. Collects the local machine's IPv4 and IPv6 addresses.
3. Formats the data and sends it to a Telegram bot using the provided **Bot Token** and **Chat ID**.

---

## **Prerequisites**

Ensure you have the following:
- **C++ Compiler**: GCC or MSVC (for Windows).
- **libcurl**: For sending HTTP requests to Telegram's API.
    - **Linux**: `sudo apt-get install libcurl4-openssl-dev`
    - **Windows**: Install a compatible version of `libcurl`.

---

## **Setup Instructions**

### **Step 1: Install libcurl (Linux Only)**

```bash
sudo apt-get update
sudo apt-get install libcurl4-openssl-dev
```

### **Step 2: Compile the Code**

Run the following command to compile the code:

**Linux/macOS**:

```bash
g++ -o sessionSkid main.cpp -lcurl
```

**Windows (MSVC)**:

Ensure `libcurl` is linked properly in your MSVC project.

### **Step 3: Update Telegram Bot Config**

In the `InfoSec()` function, update the `botConfig` with your **Bot Token** and **Chat ID**.

```cpp
TelegramBotConfig botConfig = {"<Your_Bot_Token>", "<Your_Chat_ID>"};
```

- **Bot Token**: Obtain this from [BotFather](https://core.telegram.org/bots#botfather).
- **Chat ID**: Get your **Chat ID** using the Telegram API or a bot like `userinfobot`.

### **Step 4: Run the Program**

After compiling, run the following command:

```bash
./sessionSkid
```

This will gather system info and send it to your Telegram bot.

---

## **Example Output in Telegram**

Once the program runs successfully, you'll receive a message similar to:

```
===============================
SessionSkid
- Opened: 2025-01-12 14:20:45
IPv4: 192.168.0.1
IPv6: Not Available
System: Linux
Node: my-machine
Release: 5.4.0-84-generic
Version: #94-Ubuntu SMP
Machine: x86_64
```

---

## **Troubleshooting**

- If you don't receive the message, make sure:
  - Your **Bot Token** and **Chat ID** are correct.
  - The machine running the script has internet access to connect to Telegram's API.
  - `libcurl` is installed and linked correctly.
  
- If your system doesn't return IP addresses:
  - Ensure the machine has valid network interfaces configured.
  
---

## **License**

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## **Acknowledgements**
- **libcurl**: Used to send HTTP requests to Telegram's API.
- **Windows API**: Used to gather system and network information on Windows platforms.
- **Unix `uname` Command**: Used to gather system information on Unix-like systems.
