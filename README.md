# Implementing PX4 simple automatic control module

基於jMAVSim的PX4自動控制模組模擬

## 壹、基本說明
**一、目標：**
為了實現本專案，計畫設計一套以*.cf檔案格式描述桁架結構的輸入機制。系統將讀取該檔案內容，依據定義的桁架元素建立整體勁度矩陣，並透過矩陣反斜槓運算子進行反矩陣求解。最終，程式將輸出包含桿件位移、內力與應力分析結果的*.txt報告，以供後續查閱與應用。

**二、開發環境：**
以下是開發該平台所採用的環境：
* 虛擬機：VirtualBox
* 程式語言：C++
* 程式編輯器：Visual Studio Code

## 貳、操作說明
**一、操作程式方式：**
1. 從[XQuartz官方網站](https://www.xquartz.org) 下載最新版本進行安裝（開發設備為搭載Intel處理器的MacBook Pro）。
2. 在Docker環境中拉取最新的px4io/px4-dev-simulation-jammy映像檔。
3. 啟動XQuartz，並允許網絡連接。（XQuartz版本為XQuartz 2.8.5，請由選單setting → 在Security標籤頁，勾選Allow connections from network clients）
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


請從GitHub下載input.cf（此為範例檔案，可依照其輸入格式調整並修改內容結構）與main.c檔案，具體操作步驟如下所示：
1. 請依據input.cf的內容格式規劃並設計桁架結構（類似以下的內容），並將其儲存為副檔名為*.cf的檔案。
<br>
  <div align="center">
  	<img src="./截圖1.png" alt="Editor" width="500">
  </div>
<br>
