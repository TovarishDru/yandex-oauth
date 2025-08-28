# Yandex OAuth Demo

A simple project that demonstrates the **Yandex OAuth** mechanism.  
It allows you to perform authorization, authentication, and view your Yandex profile data.

---

## 🚀 How to Run

### 1. Create an Application in Yandex OAuth
1. Go to [oauth.yandex.ru](https://oauth.yandex.ru/).  
2. Create a new application.  
3. Request the following permissions:  
   - `login`  
   - `name`  
   - `surname`  
   - `sex`  

---

### 2. Installation and Startup

```bash
# Clone the repository
git clone https://github.com/TovarishDru/yandex-oauth.git
cd yandex-oauth

# Build containers
docker compose build

# Start in the background
docker compose up -d
