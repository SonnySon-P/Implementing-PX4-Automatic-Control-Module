# Implementing PX4 simple automatic control module

基於Gazebo的PX4自動控制模組模擬。

## 壹、基本說明
**一、目標：**
以下為一個簡單的PX4模組範例程式碼，旨在實現指定的任務流程：起飛 → 前進1公尺 → 左右各來回1公尺 → 執行360度翻轉 → 降落。該模組可在PX4平台上運行，並透過Gazebo模擬器進行模擬測試。

**二、開發環境：**
以下是開發該平台所採用的環境：
* 虛擬機：VirtualBox
* 作業系統：Ubuntu 22.04.5
* 程式語言：C++
* 程式編輯器：Visual Studio Code

## 貳、操作說明
**一、安裝步驟：**
1. PX4與Gazebo安裝步驟，可依照[Setting up a Developer Environment](https://docs.px4.io/main/en/dev_setup/dev_env_linux_ubuntu) 進行安裝。
2. QGroundControl安裝步驟，可依照[QGroundControl Installation](https://docs.px4.io/main/en/dev_setup/dev_env_linux_ubuntu) 進行安裝。

**二、檔案說明：**
```bash
.
├── PX4-Autopilot/
└── src/
     └──  modules/
           └──  px4_simple_app/
                 ├── px4_simple_app.c
                 └── CMakeLists.txt
```
**三、編譯檔案：**
```shell
make clean
make px4_sitl_default
```

1. 開啟QGroundControl
2. 啟動SITL模擬
```shell
make px4_sitl gz_x500
```
1. 模擬截圖
<br>
  <div align="center">
  	<img src="./截圖.png" alt="Editor" width="500">
  </div>
<br>

## 參、反思
本專案主要在2019年款MacBook Pro上安裝VirtualBox執行PX4模擬環境。然而，由於資源限制，系統經常出現當機或效能瓶頸的情況。儘管無人機模擬可依照程式指令執行相關任務，實務上卻常遭遇模擬中斷、傳感器數值異常等問題，進而影響模擬流程與除錯效率。若本專案程式碼中仍存有疏漏，尚祈讀者不吝指正，並敬請包涵。

```text
add_subdirectory(src/modules/simple_mission)
add_subdirectory(src/examples/px4_simple_2)
```
