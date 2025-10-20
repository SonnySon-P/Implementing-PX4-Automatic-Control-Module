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
**四、檔案說明：**
```bash
.
├── PX4-Autopilot/
└── src/
     └──  modules/
           └──  simple_mission/
                 ├── simple_mission.cpp
                 └── CMakeLists.txt
```
```text
add_subdirectory(src/modules/simple_mission)
```
```shell
make clean
make px4_sitl_default
```

1. 開啟QGroundControl
2. 啟動SITL模擬
```shell
make px4_sitl gz_x500
```


請從GitHub下載input.cf（此為範例檔案，可依照其輸入格式調整並修改內容結構）與main.c檔案，具體操作步驟如下所示：
1. 請依據input.cf的內容格式規劃並設計桁架結構（類似以下的內容），並將其儲存為副檔名為*.cf的檔案。
<br>
  <div align="center">
  	<img src="./截圖1.png" alt="Editor" width="500">
  </div>
<br>
