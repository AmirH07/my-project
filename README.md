# my-project
404107305

این متن کامل فایل **`README.md`** است. کافی است روی دکمه‌ی **Copy** بالای کادر زیر کلیک کنید و آن را مستقیماً در ادیتور گیت‌هاب یا فایل `README.md` خود پیست (Paste) کنید:

# 🎓 University Educational Management System (EDU System)
### سیستم جامع مدیریت آموزشی و انتخاب واحد دانشگاه

[![Language: C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Standard: C99/C11](https://img.shields.io/badge/Standard-C99%2FC11-green.svg)]()
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

---

## 🌐 Language Options / زبان‌ها
- [English Documentation](#-english-documentation)
- [مستندات فارسی](#-مستندات-فارسی)

---

<a name="-english-documentation"></a>
## 🇬🇧 English Documentation

### 📌 Overview
The **University Educational Management System (EDU System)** is a comprehensive, console-based academic management software written in **C**. It models a university environment with multi-role access (Educational Deputy, Professor, and Student), automated persistence to local disk files, a robust security verification system, and a dynamic state machine managing semester phases.

---

### ✨ Key Features

#### 🛡️ 1. Multi-Role Authentication & Security
- **Role-Based Portals**: Separate dashboards and privileges for **Educational Deputy (Admin)**, **Professors**, and **Students**.
- **Security Question Verification**: Integrated 4-question security challenge mechanism for self-service password recovery.
- **Data Persistence**: Automatically loads and saves state across runs using localized text databases (`system.txt`, `users.txt`, `catalog.txt`, `offerings.txt`, `enrollments.txt`).

#### 🏛️ 2. Educational Deputy (Admin) Dashboard
- **User Management**: Add, list, and manage students and professors.
- **Master Course Catalog**: Maintain global university courses (units, prerequisites, departments).
- **Approval Workflow**: Review and approve/reject course offerings submitted by professors.
- **Semester Phase Engine**: Advance through the four core phases of an academic term.
- **Manual Overrides**: Force-enroll students, override capacities, or reallocate course slots.

#### 👨‍🏫 3. Professor Dashboard
- **Course Offering Requests**: Submit new course offerings with schedules, capacity, and exam dates.
- **Capacity & Cancellation Requests**: Request seat expansion or cancel course offerings.
- **Grade Entry Module**: Submit and record student final grades (0.00 – 20.00 scale) during designated grade entry phases.
- **Survey Evaluation**: Review anonymous student survey ratings and teaching evaluations.
- **LMS Module**: Publish class announcements and upload coursework/exam materials.

#### 👨‍🎓 4. Student Dashboard
- **Smart Course Enrollment**: Search and register for approved course offerings during active enrollment phases.
- **Academic Transcript & GPA**: Real-time calculated overall GPA based on completed course units and grades.
- **Course & Professor Survey**: Submit feedback and scores (1 to 5 scale) for active course offerings.
- **Doctoral / Graduate Thesis**: Register research proposals and track advisor defense statuses.
- **LMS Portal**: View announcements and download lecture materials.

---

### 🔄 Semester Phase Workflow System

The core operational state of the system is governed by a **4-Phase State Engine**:


```

[ Phase 1: Course Offering ] ➔ Professors submit courses & Deputy approves
↓
[ Phase 2: Student Enrollment ] ➔ Students register for approved courses
↓
[ Phase 3: Classes & Exams ] ➔ Course schedules locked & Learning occurs
↓
[ Phase 4: Grade Entry ] ➔ Professors input final grades & Transcripts update
↓
(Cycle Restarts & Active Semester Increments: e.g., 1403-1 ➔ 1403-2)

```

---

### 🔑 Default Demo Credentials

| Role | Username / ID | Default Password | Access Level |
| :--- | :--- | :--- | :--- |
| **Educational Deputy** | `admin` | `admin123` | Full Administrative Privileges |
| **Professor** | `p98765` | `prof123` | Course Offering & Grading |
| **Student** | `401123456` | `student123` | Course Enrollment & Transcript |

---

### ⚙️ Compilation & Running

#### Prerequisites
- Any standard C compiler supporting **C99** or later (e.g., `gcc`, `clang`, `MSVC`).

#### Steps
1. **Clone the Repository**:
   ```bash
   git clone [https://github.com/your-username/university-edu-system.git](https://github.com/your-username/university-edu-system.git)
   cd university-edu-system

```

2. **Compile the Source Code**:
```bash
gcc -Wall -std=c99 main.c -o edu_system

```


3. **Execute the Application**:
* **Linux / macOS**:
```bash
./edu_system

```


* **Windows (CMD / PowerShell)**:
```cmd
edu_system.exe

```





---

### 📁 File Database Structure

The application automatically creates and manages the following flat-file databases:

* `system.txt` — Stores the active semester code and current phase enum state.
* `users.txt` — Stores student and professor profiles, credentials, and demographic data.
* `catalog.txt` — Contains the master catalog of all accredited university courses.
* `offerings.txt` — Contains current semester offered courses, capacity, schedules, and approval status.
* `enrollments.txt` — Tracks student course enrollments and assigned numerical grades.

---

---

## 🇮🇷 مستندات فارسی

### 📌 درباره پروژه

**سیستم جامع مدیریت آموزشی دانشگاه (EDU System)** یک نرم‌افزار ایزوله تحت کنسول به زبان **C** است که فرآیندهای پیچیده آموزشی دانشگاه از جمله ثبت‌نام، انتخاب واحد، مدیریت اساتید و دانشجویان، ارزیابی، سیستم LMS و ثبت نمرات را شبیه‌سازی می‌کند. این برنامه بر پایه معماری مدیریت فازهای ترم تحصیلی (State Machine) و ذخیره‌سازی خودکار داده‌ها در فایل طراحی شده است.

---

### ✨ امکانات و قابلیت‌های اصلی

#### 🛡️ ۱. مدیریت احراز هویت و امنیت

* **سطوح دسترسی سه‌گانه**: داشبورد مجزا برای **معاونت آموزشی**، **اساتید** و **دانشجویان**.
* **بازیابی رمز عبور امن**: سیستم بازیابی کلمه عبور بر اساس پاسخ به ۴ سوال امنیتی پیش‌فرض.
* **ذخیره‌سازی پایداری داده‌ها (Persistence)**: بارگذاری و ذخیره‌سازی خودکار اطلاعات در فایل‌های متنی پایگاه داده (`system.txt`, `users.txt` و...).

#### 🏛️ ۲. پنل معاونت آموزشی (مدیر سیستم)

* **مدیر کاربران**: افزودن و مشاهده لیست دانشجویان و اساتید به همراه صدور خودکار حساب کاربری.
* **چارت درسی (Master Catalog)**: تعریف دروس دانشگاهی، تعداد واحدها، دانشکده و پیش‌نیازها.
* **تایید ارائه دروس**: بررسی، تایید یا رد درخواست‌های ارائه درس توسط اساتید.
* **مدیریت فازهای ترم**: تغییر و پیشبردن فازهای مختلف ترم تحصیلی.
* **دسترسی‌های مدیریتی (Override)**: ثبت‌نام دستی دانشجو، تغییر ظرفیت کلاس‌ها و مدیریت اضطراری.

#### 👨‍🏫 ۳. پنل اساتید

* **درخواست ارائه درس**: تعریف کلاس جدید به همراه ظرفیت، برنامه هفتگی و تاریخ امتحان.
* **مدیریت ظرفیت و لغو درس**: ارسال درخواست افزایش ظرفیت یا لغو یک ارائه درس.
* **ثبت نمرات**: ورود نمرات پایانی دانشجویان (در بازه ۰٫۰۰ تا ۲۰٫۰۰) در فاز ثبت نمرات.
* **نتایج ارزشیابی**: مشاهده نظرسنجی‌ها و ارزشیابی‌های کیفی دانشجویان.
* **سامانه LMS**: ارسال اطلاعیه‌های کلاسی و بارگذاری تکالیف و منابع درسی.

#### 👨‍🎓 ۴. پنل دانشجویان

* **انتخاب واحد هوشمند**: جستجو و اخذ دروس تایید شده در زمان فاز انتخاب واحد.
* **کارنامه و معدل کل**: محاسبه خودکار و آنلاین معدل کل بر اساس واحدهای گذرانده‌شده و نمرات.
* **ارزشیابی اساتید**: ثبت نظر و امتیازدهی (۱ تا ۵) به کیفیت تدریس اساتید.
* **مدیریت پایان‌نامه (مقاطع تکمیلی)**: ثبت موضوع پایان‌نامه دکتری/ارشد و پیگیری وضعیت دفاع.
* **پورتال آموزشی LMS**: مشاهده اعلامیه‌ها و دریافت فایل‌های درسی.

---

### 🔄 چرخه حیات و فازهای نیم‌سال تحصیلی

سیستم بر اساس یک **ماشین وضعیت ۴ مرحله‌ای** عمل می‌کند:

```
[ فاز ۱: ارائه دروس ] ➔ درخواست ارائه توسط اساتید و تایید توسط معاونت آموزشی
           ↓
[ فاز ۲: انتخاب واحد ] ➔ امکان اخذ دروس تایید شده توسط دانشجویان
           ↓
[ فاز ۳: کلاس‌ها و امتحانات ] ➔ قفل شدن تغییرات انتخابی و برگزاری کلاس‌ها
           ↓
[ فاز ۴: ثبت نمرات ] ➔ ورود نمرات توسط اساتید و به‌روزرسانی کارنامه
           ↓
(ارتقای ترم تحصیلی و شروع مجدد چرخه: مثلاً ۱۴۰۳-۱ ➔ ۱۴۰۳-۲)

```

---

### 🔑 اطلاعات ورود پیش‌فرض جهت تست

| نقش | نام کاربری / شماره شناسه | رمز عبور پیش‌فرض | سطح دسترسی |
| --- | --- | --- | --- |
| **معاونت آموزشی** | `admin` | `admin123` | دسترسی کامل مدیریتی |
| **استاد** | `p98765` | `prof123` | ارائه درس و ثبت نمره |
| **دانشجو** | `401123456` | `student123` | انتخاب واحد و مشاهده کارنامه |

---

### ⚙️ نحوه کامپایل و اجرا

#### پیش‌نیازها

* کامپایلر استاندارد زبان C با پشتیبانی از **C99** یا بالاتر (مانند `GCC` یا `Clang`).

#### مراحل اجرا

1. **کلون کردن ریپوزیتوری**:
```bash
git clone [https://github.com/your-username/university-edu-system.git](https://github.com/your-username/university-edu-system.git)
cd university-edu-system

```


2. **کامپایل سورس کد**:
```bash
gcc -Wall -std=c99 main.c -o edu_system

```


3. **اجرای برنامه**:
* **لینوکس / مک**:
```bash
./edu_system

```


* **ویندوز**:
```cmd
edu_system.exe

```





---

### 📝 ساختار فایل‌های پایگاه داده

برنامه به طور خودکار فایل‌های زیر را جهت ذخیره‌سازی داده‌ها مدیریت می‌کند:

* `system.txt`: نگهداری ترم فعال و شماره فاز جاری سیستم.
* `users.txt`: مشخصات کامل دانشجویان، اساتید و حساب‌های کاربری.
* `catalog.txt`: کاتالوگ جامع دروس مصوب دانشگاه.
* `offerings.txt`: دروس ارائه شده در ترم جاری به همراه ظرفیت و برنامه کلاسی.
* `enrollments.txt`: لیست انتخاب واحدها و نمرات ثبت‌شده دانشجویان.

---

## 📜 License

This project is open-source and available under the [MIT License](LICENSE).

```

```
