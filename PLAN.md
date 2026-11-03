# خطة الشغل — Boombanana IRL

آخر تحديث: 2 نوفمبر 2026
الديدلاين: **30 نوفمبر 2026** (قدّم قبل 20 نوفمبر)

---

## 0) قبل أي جلسة شغل 🔴

1. افتح **Lapse**: https://lapse.hackclub.com
2. دوس **Start**
3. اشتغل
4. دوس **Stop** لما تخلص

> KiCad و Onshape مش بيتتبعوا بـ Hackatime — لازم Lapse.

---

## 1) تحديث BOM.csv على GitHub (5 دقايق)

الملف محفوظ محليًا في: `D:\hack-club-projects\BOM.csv`

روح على GitHub → `BOM.csv` → القلم ✏️ → `Ctrl+A` → `Delete` → الصق المحتوى الجديد من الملف المحلي → Commit:

```
Update BOM: 5 RGB LEDs for cable indicator
```

**التغيير:** RGB LED من 4 لـ 5، والمجموع **66.24$**.

---

## 2) KiCad — Schematic (ساعتين)

### الإعداد
```
New Project → D:\hack-club-projects\hardware\boombanana
Schematic Editor
```

### القطع اللي تحطها (Place Symbol — اختصار A)

| القطعة | الكمية | ملاحظة |
|---|---|---|
| ESP32 DevKit V1 | 1 | لو مش موجود في المكتبة، استخدم `Connector_Generic:Conn_01x15` |
| Banana Jack | 8 | 4 أزواج |
| Toggle Switch (SPDT) | 4 | |
| RGB LED | 5 | 4 للمفاتيح + 1 للأسلاك |
| Buzzer | 1 | |
| Push Button | 1 | Start |
| TFT ILI9341 | 1 | استخدم connector 14-pin |

### التوصيلات (Wire — اختصار W)

| من | إلى |
|---|---|
| Jack 1A | GPIO 13 |
| Jack 2A | GPIO 14 |
| Jack 3A | GPIO 16 |
| Jack 4A | GPIO 17 |
| كل Jack B | GND (مشترك) |
| Switch 1–4 | GPIO 32 / 33 / 34 / 35 |
| Switch (الطرف التاني) | GND |
| RGB LED (مفاتيح) | GPIO 25 / 26 / 27 (أو عبر expander) |
| Cable LED | GPIO 23 |
| Buzzer | GPIO 18 |
| Start Button | GPIO 4 |
| TFT CS / DC / RST | GPIO 5 / 2 / 15 |
| TFT MOSI / SCK | GPIO 21 / 22 |

> ⚠️ لو الأطراف خلصت: استخدم **74HC595** أو نقل الـ LEDs لـ GPIO مختلف. ESP32 فيه أطراف كفاية — راجع `pinout` قبل الرسم.

### التصدير
```
File → Export → PDF
→ احفظه في: hardware\schematic.pdf
```

**التسليم:** `schematic.pdf` + `schematic.png` في الـ repo.

---

## 3) Onshape — الـ Case (3–4 أيام)

### الإعداد
1. **Hobbyist** account → خطة **Free** (مش trial)
2. **Lapse → Start**
3. New Document → `Boombanana_IRL_Case`

### المقاسات الأساسية

```
العلبة: 200 × 140 × 45 مم
سماكة الجدار: 3 مم
```

### الخطوة 1 — الصندوق
```
Sketch (Front plane) → Rectangle 200 × 140
Extrude → 45 mm (Symmetric)
Shell → 3 mm  (افتح الوش العلوي)
```

### الخطوة 2 — الحواف
```
Fillet → 5 mm على الأربع زوايا الرأسية
Fillet → 2 mm على الحواف العلوية
```

### الخطوة 3 — الفتحات (Hole / Extrude Cut)

| الفتحة | المكان | المقاس |
|---|---|---|
| شاشة TFT | فوق شمال | **52 × 40** مستطيل بحواف 2 مم |
| زر Start | فوق يمين | **12 مم** |
| Buzzer | فوق وسط | **12 مم** |
| Toggle ×4 | صف النص على x = 55 / 90 / 125 / 160 | **6.2 مم** |
| LED ×4 | فوق كل مفتاح بـ 10 مم | **5.2 مم** |
| Cable LED | فوق الأسلاك يمين | **10.2 مم** |
| Banana Jack ×8 | تحت، 2 صف × 4 عمود | **قِس المقبس + 0.3 مم** |
| USB | جنب فوق | **12 × 8** مع recess 2 مم |

> 💡 **اختبار قبل الطباعة:** اطبع لوح صغير 40×40 فيه فتحات 8 / 9 / 10 / 11 مم، وجرّب المقبس الحقيقي فيه. ده بيوفر عليك طباعة علبة كاملة غلط.

### الخطوة 4 — النقاط البارزة (Tactile Dots)

```
تحت كل مفتاح بـ 12 مم:
Switch 1 → نقطة واحدة
Switch 2 → نقطتين
Switch 3 → 3 نقاط
Switch 4 → 4 نقاط

كل نقطة: قطر 2 مم، ارتفاع 1 مم، المسافة بينهم 4 مم
```

> استخدم **Extrude** على sketch فيه دواير — متستخدمش Hole.

### الخطوة 5 — الحواجز
```
Lip / Ridge بين قسم المفاتيح وقسم الأسلاك: ارتفاع 3 مم، عرض 2 مم
حافة بارزة 5 مم حوالين المقابس
```

دي بتساعد الأعمى يلاقي الأقسام بإيده.

### الخطوة 6 — تثبيت القطع (Bosses)
```
4 بوصات (bosses) للشاشة: قطر 6 مم، ارتفاع 8 مم، فتحة M2.5
4 بوصات للـ ESP32: قطر 6 مم، ارتفاع 8 مم، فتحة M3
```

### الخطوة 7 — التصدير
```
Right-click على Part Studio → Export → STL
```

### إعدادات الطباعة
```
PLA — طبقة 0.2 مم — جدران 3 — تعبئة 20%
القطع اللي تحتاج دقة أعلى: 0.12 مم
```

---

## 4) الكود (VS Code) — بعد ما الـ Build ينجح

### الترتيب
```
1. Buzzer + Timer      ← ابدأ هنا (أسهل حاجة)
2. TFT (timer + strikes)
3. Cable module
4. Switch module
```

### ملفات المشروع
```
src\main.cpp          ← البرنامج الرئيسي
lib\buzzer\           ← مكتبة الصوت
lib\cable\            ← وحدة الأسلاك
lib\switch_module\    ← وحدة المفاتيح
include\config.h      ← أرقام الأطراف
```

### أرقام الأطراف (config.h)
```cpp
#define JACK_1 13
#define JACK_2 14
#define JACK_3 16
#define JACK_4 17
#define BUZZER 18
#define CABLE_LED_R 25
#define CABLE_LED_G 26
#define CABLE_LED_B 27
#define START_BTN 4
#define TFT_CS 5
#define TFT_DC 2
#define TFT_RST 15
```

### البازر على Arduino Core 2.x (espressif32 6.12.0)
```cpp
ledcSetup(0, 2000, 8);     // channel, freq, resolution
ledcAttachPin(18, 0);      // pin, channel
ledcWriteTone(0, 440);     // نغمة
ledcWriteTone(0, 0);       // سكوت
```

### البناء
```
دبل كليك على: D:\hack-club-projects\build.bat
```

---

## 5) الـ Repo — الشكل النهائي

```
Boombanana_IRL/
├── README.md
├── BOM.csv
├── LICENSE
├── build.bat
├── platformio.ini
├── src/
│   └── main.cpp
├── include/
├── hardware/
│   ├── schematic.pdf
│   ├── wiring.png
│   └── boombanana.kicad_sch
├── case/
│   ├── case.stl
│   └── case-screenshot.png
├── manual/
│   ├── manual.pdf
│   └── pages/*.jpg
├── images/
│   ├── paper-prototype.jpg
│   └── final-build.jpg
└── docs/
    └── game-design.md
```

---

## 6) الـ README النهائي — العناصر المطلوبة

- [ ] وصف المشروع (سطرين)
- [ ] صورة أو GIF
- [ ] فيديو الديمو
- [ ] شرح الأدوار الثلاثة
- [ ] الوحدات (Cable + Switch)
- [ ] Hardware section + لينك BOM
- [ ] لينك Onshape (public document)
- [ ] Status: paper prototype ✅ / hardware 🔧
- [ ] Credits: BOMBANANA! by Lefto Studio
- [ ] Future Work: PCB، DFPlayer، Direction، Calculation

---

## 7) الجدول الزمني

| الأسبوع | الشغل |
|---|---|
| 2–5 نوفمبر | KiCad Schematic + Onshape Case |
| 6–10 نوفمبر | طباعة الـ Case + طلب القطع |
| 11–17 نوفمبر | كود (Buzzer → TFT → Cable → Switch) |
| 18–24 نوفمبر | تجميع + كتيب + فيديو |
| **19–20 نوفمبر** | **تقديم Forge** ✅ |
| 25–30 نوفمبر | احتياطي (لو حاجة اتأخرت) |

---

## 8) قواعد ثابتة — متكسرهاش

| القرار | الحالة |
|---|---|
| الشاشة: 2.4" TFT ILI9341 | 🔒 مقفول |
| المقابس: 8 (4 أزواج) | 🔒 مقفول |
| اللمبات: 5 RGB | 🔒 مقفول |
| المقاس: 200 × 140 × 45 | 🔒 مقفول |
| الأرقام: نقاط بارزة ثابتة | 🔒 مقفول |
| الألوان: عشوائي بالكود | 🔒 مقفول |
| مفيش PCB في v1 | 🔒 مقفول |
