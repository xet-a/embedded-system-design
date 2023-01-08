# embedded_system_design
PNU CSE 2022 'Embedded System Design & Lab' Repository

부산대학교 정보컴퓨터공학부 2022 '임베디드 시스템 설계 및 실험' 수업 자료 및 텀 프로젝트를 정리한 저장소입니다.

***

### ✔ 프로젝트 소개

<p align="center"><img src="https://user-images.githubusercontent.com/78337522/211184217-bed483a1-86fa-458c-8043-fb45cc59dadf.jpg" width="350" height="450"/><img src="https://user-images.githubusercontent.com/78337522/211184210-dbc1495f-2377-435e-9da3-69434674df18.jpg" width="260" height="450"/></p>
<p align="center"><img src="https://user-images.githubusercontent.com/78337522/211187073-ea6c0848-aee0-4185-9167-d3d0075d0f29.gif" width="200" height="300"/><img src="https://user-images.githubusercontent.com/78337522/211187376-79f12906-a77e-477e-8272-9dcb4f398b08.gif" width="500" height="300"/></p>
<p align="center"><img src="https://user-images.githubusercontent.com/78337522/211187568-06748f06-8ede-4795-a332-f3368dc3d01d.gif" width="500" height="300"/></p>


**임베디드 시스템을 이용한 핀볼 게임 구현 프로젝트**
- 수업 시간에 배운 보드 기능과 여러 가지 센서들을 이용한 하드웨어를 개발하였습니다.
- Bluetooth 및 통신 관련 기능, 인터럽트 등을 활용하였습니다.
- 하나의 센서가 다른 센서 값을 이용할 수 있도록 센서 간 의존성이 있도록 개발하였습니다.
- 스마트폰으로 실제 핀볼 판에 손을 대지 않고 조작할 수 있습니다.
<br>

- [프로젝트 제안서](term_project/doc/6%EC%A1%B0%20%ED%85%80%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20%EC%A0%9C%EC%95%88%EC%84%9C.pdf)
- 자세한 [구현 과정](term_project/progress.md) 및 [최종 보고서](term_project/doc/6조_텀프로젝트_최종보고서.pdf)
- [시연 영상](https://drive.google.com/drive/folders/1gqu4sN3soiElaZDJ48mgq3Tf-cr_iFLQ?usp=sharing)

***

### 😎 실험 환경

**실험 장비**

- STM32(Cortex-M3)
- JTAG(Joint Test Action Group)
- Android Device

**소프트웨어**
- IAR Embedded Workbench for Arm
- Android Studio

***

### 📌 디렉토리 구조 소개
```javascript
📦embedded_system_design
 ┣ 📂android_bluetooth          // 블루투스 안드로이드 앱 프로젝트
 ┣ 📂reference                  // 데이터 시트 및 레퍼런스 파일
 ┃ ┣ 📜STM32F107VCT6_schematic.pdf
 ┃ ┣ 📜stm32_Datasheet.pdf
 ┃ ┗ 📜stm32_ReferenceManual.pdf
 ┣ 📂term_project               // 텀 프로젝트 최종 폴더
 ┃ ┣ 📂doc                      // 제안서 및 최종 보고서
 ┃ ┃ ┣ 📜6조 텀 프로젝트 제안서.pdf
 ┃ ┃ ┣ 📜6조_센서신청.xlsx
 ┃ ┃ ┣ 📜6조_텀프 제안서 보충_인터럽트 적용 기능.docx
 ┃ ┃ ┗ 📜6조_텀프로젝트_최종보고서.pdf
 ┃ ┣ 📂termp_o                   // 전산실에서 작업한 프로젝트 폴더
 ┃ ┣ 📂user                      // 전산실 외 다른 곳에서는 해당 폴더만 복사 후 새 프로젝트에 파일을 따로 추가해주세요 (버전 차이)
 ┣ 📜.gitignore
 ┣ 📜lab.zip                     // 실험 주차별 제출 파일, 예비 발표 자료
 ┗ 📜수업자료 및 시험 정리.zip
```

- `lab.zip` 와 `수업자료 및 시험정리.zip` 파일은 비밀번호가 설정되어 있습니다. 필요하신 분은 따로 연락주세요.


***

### 👩🏻‍💻👨🏻‍💻 팀원 (화요일 6조)
- 김선규
- 이동현
- 이주승
- 이지현
- 최세희
