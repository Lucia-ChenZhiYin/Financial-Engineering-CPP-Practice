// 找到 HTML 中準備放置筆記的區域
const notesContainer = document.querySelector("#notes-content");

// 取得 HTML 設定的 Markdown 檔案位置
const markdownPath = notesContainer.dataset.markdown;

// 讀取 Markdown 檔案
fetch(markdownPath)
    .then(response => {
        // 確認檔案是否成功讀取
        if (!response.ok) {
            throw new Error("無法讀取 Markdown 檔案");
        }

        return response.text();
    })
    .then(markdown => {
        // 將 Markdown 轉換成 HTML，放進網頁
        notesContainer.innerHTML = marked.parse(markdown);
        // 將 Markdown 中的數學公式轉換成數學排版
        MathJax.typesetPromise([notesContainer]);
    })
    .catch(error => {
        // 如果讀取失敗，顯示錯誤訊息
        notesContainer.textContent = "筆記載入失敗，請檢查檔案路徑。";
        console.error(error);
    });
