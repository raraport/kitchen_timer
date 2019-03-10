#include <M5Stack.h>
// ===== 変数の宣言・初期化 ===== //

// あれ、これ使ってないよね
boolean but_A = false, but_LEFT = false, but_RIGHT = false;

// この値をmillis()の値が上回ったら1秒カウント
uint32_t targetTime = 0;
// void loop() {
//   if (targetTime < millis()) {
//     targetTime = millis() + 1000;
//     // ここが１秒おきに実行される
//   }
// }
// ＊ millis(): 起動時から現在までの時間 (ミリ秒)

//画面上の「分」「秒」
byte omm = 99, oss = 99;

// プログラム上の「分」
uint8_t mm = 3;

 // プログラム上の「秒」
int8_t ss= 0

// 停止中フラグ
// true → タイマー停止状態
// ボタンを押したときの動作に関わる
bool hasStop = true;

// タイマー終了フラグ
// true → タイマー終了
// 「Timer Stop」の表示に関わる
bool hasEnd = false;

void setup() {
  // ===== 画面の初期化など ===== //
  M5.begin();
  M5.Lcd.fillScreen(TFT_LIGHTGREY);

  // ↓これタイマースタート時(68行目)にしたほうがいいと思う
  targetTime = millis() + 1000;

  M5.Lcd.setTextFont(2);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(TFT_BLACK, TFT_LIGHTGREY);
  M5.Lcd.setCursor( 60, 200);
  M5.Lcd.print("M");
  M5.Lcd.setCursor( 150, 200);
  M5.Lcd.print("S");
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor( 240, 200);
  M5.Lcd.print("START");
  M5.Lcd.setCursor( 240, 220);
  M5.Lcd.print("/STOP");

  Serial.println("START");
}

void loop() {
  // ==== ボタンに対しての処理 ==== //
  if(M5.BtnC.wasPressed() && !hasStop) {
  // ●「Cボタン」が押された場合　(@タイマー作動中) → ストップ
    hasStop = true; // 停止中フラグをtrue
  } else if (M5.BtnC.wasPressed() && hasStop) {
  // ●「Cボタン」が押された場合 (@タイマー停止状態) → スタート
    hasStop = false;  // 停止中フラグをfalse
    // スタートした時刻を基準にカウントするため以下を追記してみて
    // targetTime = millis() + 1000;
  }

  if(M5.BtnB.wasPressed() && hasStop) {
  // ●「Bボタン」が押された場合 (@タイマー停止状態) →　「秒」を設定
    ss++; //「秒」を+1

    // hasEndをfalseにすることで
    // 「Timer Stop」が表示されていた場合に表示が消される
    hasEnd = false; // 「Timer Stop」の表示を消す
    if (ss == 60){
      ss = 0; // 「秒」の値が60のときカウントリセット
    }
    draw_seconds(ss); // 「秒」を描画
  }
  
  if(M5.BtnA.wasPressed() && hasStop) {
  // ●「Aボタン」が押された場合 (@タイマー停止状態) → 「分」を設定
    mm++; // 「分」を+1
    hasEnd = false; // 「Timer Stop」の表示を消す
    if (mm == 60){
      mm = 0; // 「分」の値が60のときカウントリセット
    }
    draw_minutes(mm); // 「分」を描画
  }
  M5.update();

  // ==== 以下は１秒おきに実行する内容 ==== //
  if (targetTime < millis()) {
  // 起動時からの時間がtargetTimeを超えた場合

    // さらに1秒後をtargetTimeに設定
    targetTime = millis() + 1000;

    if (!hasStop){
    // ●タイマー作動中の場合
      ss--; //「秒」を-1
      if (ss == -1) {
        if (mm == 0) {
        // 「秒」が 0 から -1 に変わり 、「分」が 0 のとき。
        // (00:00 → ピピピピ)
          ss = 0; // 「秒」を 0 にする
          hasStop = true; // 停止中フラグをtrueにする
          hasEnd = true; // 「Timer Stop」を表示するためにtrueにする
          M5.Speaker.beep();
        } else {
        // 「秒」が 0 から -1 に変わり、「分」は 0 じゃないとき。
        // e.g.(01:00 → 0:59)
          ss = 59; // 「秒」を 59 にする
          omm = mm; // ん、これ必要なのかな…
          mm--; // 「分」を -1
        }
      }
    }

    if (hasEnd) {
    // ●タイマー終了後の場合
      // timerストップの表示
      M5.Lcd.setCursor( 100, 150);
      M5.Lcd.setTextFont(2);
      M5.Lcd.print("Timer Stop");
    } else {
      // 表示を消す
      M5.Lcd.setCursor( 100, 150);
      M5.Lcd.setTextFont(2);
      M5.Lcd.print("          ");
    }

    // === 以降は７セグの描画 === //
    // 7seg font
    M5.Lcd.setTextFont(7);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(TFT_BLACK, TFT_LIGHTGREY);

    // 分の更新
    if (omm != mm) {
    // 「分]の値が画面の内容と違う場合に再描画
      omm = mm;
      draw_minutes(mm);　// 再描画
    }

    // 秒の更新
    if (oss != ss) {
    // 「秒]の値が画面の内容と違う場合に再描画    
      oss = ss;

      // インジケーターの点滅
      //↓「ss % 2 == 1」の「== 1」が省略されてる
      if (ss % 2 && !hasStop) {
      // 「秒」が奇数のとき
        M5.Lcd.setCursor( 150, 40);
        M5.Lcd.setTextColor(TFT_LIGHTGREY, TFT_LIGHTGREY);
        M5.Lcd.print(":");
        M5.Lcd.setTextColor(TFT_BLACK, TFT_LIGHTGREY);
      }
      else {
      // 「秒」が偶数のとき
        M5.Lcd.setCursor( 150, 40);
        M5.Lcd.print(":");
      }
      
      draw_seconds(ss); // 再描画
    }
  }
}

// ===== 数字を描画するための関数 ===== //
// 分の表示
void draw_minutes(int mm) {
  if (mm < 10) {
  // 10未満の場合は0のあとに描画
  // e.g.(4 → 04)
    M5.Lcd.setCursor( 0, 40);
    M5.Lcd.print("0");
    M5.Lcd.print(mm);
  } else {
    M5.Lcd.setCursor( 0, 40);
    M5.Lcd.print(mm);
  }
}

// 秒の表示
void draw_seconds(int ss) {
  if (ss < 10) {
  // 上に同じ
    M5.Lcd.setCursor( 180, 40);
    M5.Lcd.print("0");
    M5.Lcd.print(ss);
  } else {
    M5.Lcd.setCursor( 180, 40);
    M5.Lcd.print(ss);
  }
}
