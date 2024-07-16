# MapleStory v113 Hook DLL

## 版本說明

此版本為113版本，使用 `ijl15` 啟動 `MapleStory.exe`。請按照以下步驟操作：

1. 選擇 `x86` 以及 `Release` 編譯所得到的 `ijl15.dll`。
2. 將編譯生成的 `ijl15.dll` 覆蓋到 v113 的遊戲資料夾中。
3. 覆蓋後即可啟動 `MapleStory.exe`。

## 當前版本功能

本版本目前實現了以下功能：

- 添加了 Detours 庫以便更可靠地掛鉤函數。
- 實現了 `MonitorDNS` 函數以持續監控並掛鉤 `gethostbyname`。
- 在 `DllMain` 中創建了一個新線程以確保 `MonitorDNS` 持續運行。
- 添加了多個 `MessageBox` 調用以調試並確認掛鉤過程。
- 確保掛鉤函數 `hooked_gethostbyname` 處理特定的 DNS 查詢並在需要時返回自定義響應。
- 提高了掛鉤實現的可靠性和穩定性。

## 編譯說明

在編譯此專案之前，請確保從 Detours 庫獲取並引入 `detours.h` 和相關的 DLL 及目錄。具體步驟如下：

1. 從 Detours 庫中下載 `detours.h` 頭文件。
2. 將 Detours 相關的 DLL 和目錄引入到專案中。
3. 確保專案能夠正確引用並編譯 Detours 庫。

完成上述步驟後，即可編譯並生成最終的 `ijl15.dll`。

請按照上述步驟操作，確保正確配置和使用本專案。