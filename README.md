# Implementing PX4 simple automatic control module

基於Gazebo Classic的PX4自動控制模組模擬。

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









3. 在Docker環境中拉取最新的px4io/px4-dev-simulation-jammy映像檔。
4. 啟動XQuartz，並允許網絡連接。（XQuartz版本為XQuartz 2.8.5，請由選單setting → 在Security標籤頁，勾選Allow connections from network clients）
```shell
xhost + 127.0.0.1
xhost + localhost
```
5. 設置DISPLAY環境變數，作為Docker容器與宿主機之間的橋樑。
```shell
export DISPLAY=host.docker.internal:0
```
5. 運行Docker容器
```shell
docker run -it -e DISPLAY=host.docker.internal:0 -v /tmp/.X11-unix:/tmp/.X11-unix px4io/px4-dev-simulation-jammy:latest
```
6. 進入容器後：
```shell
apt update
apt install x11-apps -y
xclock
```
7. 如果 /home/px4/PX4-Autopilot 不存在，可能是你使用的映像未自動 clone PX4 專案。你可以手動 clone：
```shell
cd /home/px4
git clone https://github.com/PX4/PX4-Autopilot.git --recursive
cd PX4-Autopilot
```
```shell
apt update
apt install libgl1-mesa-glx libglu1-mesa libxt6 libxrender1 libxrandr2 libxinerama1 libxi6 -y
```
8. 啟動
```shell
make px4_sitl jmavsim
```

請從GitHub下載input.cf（此為範例檔案，可依照其輸入格式調整並修改內容結構）與main.c檔案，具體操作步驟如下所示：
1. 請依據input.cf的內容格式規劃並設計桁架結構（類似以下的內容），並將其儲存為副檔名為*.cf的檔案。
<br>
  <div align="center">
  	<img src="./截圖1.png" alt="Editor" width="500">
  </div>
<br>
