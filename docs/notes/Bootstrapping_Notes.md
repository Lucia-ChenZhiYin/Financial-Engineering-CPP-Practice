# Bootstrapping（拔靴法）

## 1. 核心概念（Core Idea）

Bootstrapping 是一種逐期推算零息利率（Zero Rate）的方法。

我們已知不同到期期間的債券殖利率（Yield），希望利用這些資料，逐步反推出每一期的零息利率，最後建立 Zero Curve。

### Yield 與 Zero Rate 的差異

- **Yield（殖利率）**：債券整體現金流所對應的到期殖利率。
- **Zero Rate（零息利率）**：特定到期期間單一現金流所使用的折現利率。

兩者並不一定相同。拔靴法的目的，就是利用已知的債券價格與前面已求出的零息利率，逐期反推出新的零息利率。

### 核心想法

第一期的零息利率可以直接由第一期殖利率取得：

`ZeroRate[0] = Yield[0]`

接著從第二期開始，每次利用前面已知的 Zero Rate，計算尚未求出的那一期利率。

---

## 2. Step (a)：計算債券價格 BondValue

首先，利用第 i 期債券的殖利率 `Yield[i]`，計算這張債券的價格。

債券每期支付利息 C，最後一期除了利息以外，還會償還本金 100。

因此：

`BondValue = C/(1+Yield[i]) + C/(1+Yield[i])^2 + ... + (C+100)/(1+Yield[i])^(i+1)`

### 對照程式碼

```cpp
double BondValue=0;

for(int j=0;j<=i;j++){
    double Discount=1;

    for(int k=0;k<=j;k++){
        Discount=Discount/(1+Yield[i]);
    }

    BondValue=BondValue+Discount*C;

    if(j==i){
        BondValue=BondValue+Discount*100;
    }
}
```

### 程式邏輯

1. `BondValue=0`：先將債券價格歸零。
2. 外層迴圈 `j`：逐期計算每一期現金流的現值。
3. 內層迴圈 `k`：重複除以 `(1+Yield[i])`，計算折現因子。
4. `BondValue += Discount*C`：加入當期利息的現值。
5. 當 `j==i` 時，代表最後一期，再加入本金 100 的現值。

**記憶重點：** Step (a) 是使用該期的 Yield，把整張債券的現金流折現，算出 BondValue。

---

## 3. Step (b)：計算前期利息現值 sumPV

接下來，我們要計算前面各期利息的現值。

這裡的關鍵是：前面期數的 Zero Rate 已經在先前步驟求出，因此可以直接使用。

例如，計算第三期的零息利率時：

- 第一期利息使用第一期 Zero Rate 折現。
- 第二期利息使用第二期 Zero Rate 折現。
- 第三期利息與本金則留給下一步反推。

### 數學概念

`sumPV = C/(1+ZeroRate[0]) + C/(1+ZeroRate[1])^2 + ...`

更一般地說：

`sumPV = Σ C/(1+ZeroRate[j])^(j+1)`

其中 j 從 0 到 i-1。

### 對照程式碼

```cpp
double sumPV = 0;

for(int j=0;j<i;j++){
    double PV=C;

    for(int k=0;k<=j;k++){
        PV=PV/(1+ZeroRate[i]);
    }

    sumPV = sumPV+PV;
}
```

**重要注意：這段原始程式可能有索引錯誤。**

按照逐期折現的概念，這裡應該使用 `ZeroRate[j]`，而不是 `ZeroRate[i]`。

原因是：

- `ZeroRate[j]` 是前面已經求出的第 j 期零息利率。
- `ZeroRate[i]` 是目前正在計算、尚未求出的零息利率。

因此，若要符合拔靴法的計算邏輯，這一行應為：

```cpp
PV=PV/(1+ZeroRate[j]);
```

這是程式需要留意的地方，不代表你的原始程式已經修改。

**記憶重點：** Step (b) 是把前面已知的利息現值加總，得到 sumPV。

---

## 4. Step (c) & (d)：反推出新的 Zero Rate

前面已經取得：

- `BondValue`：整張債券的價格。
- `sumPV`：前面各期利息的現值。

現在可以利用兩者的差額，反推出當期的零息利率。

### 數學推導

債券價格可以拆成兩個部分：

`BondValue = 前面各期利息現值 + 最後一期現金流現值`

因此：

`BondValue = sumPV + (C+100)/(1+ZeroRate[i])^(i+1)`

先將 `sumPV` 移到等號左邊：

`BondValue - sumPV = (C+100)/(1+ZeroRate[i])^(i+1)`

接著整理：

`(1+ZeroRate[i])^(i+1) = (C+100)/(BondValue-sumPV)`

最後開 `(i+1)` 次方：

`1+ZeroRate[i] = ((C+100)/(BondValue-sumPV))^(1/(i+1))`

得到：

`ZeroRate[i] = ((C+100)/(BondValue-sumPV))^(1/(i+1)) - 1`

### 對照程式碼

```cpp
ZeroRate[i]=pow((C+100)/(BondValue-sumPV),1.0/(i+1))-1;
```

這行程式使用 `pow()` 計算次方，將前面已知的現值扣除後，反推出當期的 Zero Rate。

**記憶重點：** Step (c)、(d) 就是利用債券價格扣除前面利息現值，再將剩餘部分視為最後一期現金流的現值，反推出新的零息利率。

---

## 5. 程式執行流程

整份程式可以分成以下幾個階段：

1. 輸入每期利息 C。
2. 輸入五期債券殖利率 Yield。
3. 設定第一期 `ZeroRate[0]=Yield[0]`。
4. 從第二期開始，逐期執行拔靴法。
5. Step (a)：計算債券價格 BondValue。
6. Step (b)：計算前面各期利息現值 sumPV。
7. Step (c)、(d)：反推出當期 Zero Rate。
8. 重複直到第五期，最後輸出五期零息利率。

### 主要變數整理

| 變數 | 用途 |
|---|---|
| `Yield[5]` | 儲存五期債券殖利率 |
| `ZeroRate[5]` | 儲存五期零息利率 |
| `C` | 每期支付的利息 |
| `BondValue` | 依照 Yield 計算的債券價格 |
| `sumPV` | 前面各期利息的現值總和 |
| `Discount` | 折現因子 |
| `PV` | 單一期利息的現值 |

---

## 6. 考試與複習重點

如果要快速回憶這份程式，可以記住以下順序：

**輸入資料 → 設定第一期 Zero Rate → 逐期計算**

- Step (a)：利用 Yield 計算 BondValue。
- Step (b)：利用前面已知的 Zero Rate 計算 sumPV。
- Step (c)、(d)：利用剩餘現值反推出新的 Zero Rate。

### 三種方法的差異

| 比較項目 | 二分法 | 牛頓法 | 拔靴法 |
|---|---|---|---|
| 核心概念 | 不斷縮小搜尋區間 | 利用導數修正答案 | 逐期反推零息利率 |
| 主要變數 | Low、High、middle | rk、f_r、df_r | Yield、ZeroRate |
| 初始設定 | 設定搜尋上下界 | 設定初始利率 | 設定第一期零息利率 |
| 更新方式 | 保留可能包含答案的一半區間 | 計算下一次利率 | 利用前期利率推算下一期 |
| 主要迴圈 | while | while | for |
| 共同目標 | 找到符合債券價格關係的利率 | 找到符合債券價格關係的利率 | 建立各期零息利率曲線 |

---

## 7. 學習心得

這次的拔靴法與前面的二分法、牛頓法不同，它並不是透過反覆修正同一個利率來尋找答案，而是利用已知的債券殖利率，從短期開始逐期推算長期的零息利率。

我認為最重要的是理解每一期的計算關係：先計算整張債券的價格，再扣除前面已經能夠計算的利息現值，最後利用剩餘的現值反推出當期 Zero Rate。

透過這個過程，可以逐步建立完整的 Zero Curve。