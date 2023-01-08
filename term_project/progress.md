## [Term Project Progress]

<br/>

|        Date       |                                                                                                                                                      Progress                                                                                                                                                     |
|:-----------------:|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|
| 22.10.20-22.10.21 | 프로젝트 기획 회의 - 주제 선정 |
| 22.10.22-22.10.23 | 프로젝트 제안서 작성 |
| 22.10.24-22.10.25 | 인터럽트 기능 구체화 |
|      22.11.22     | 센서(모터, 진동) 동작 확인, 판 구성 틀 잡기, 역할 분배 |
|      22.11.29     | 코드 구현 팀 : 12.02(금)에 진행 상황 공유(회의)<br>- 블루투스 앱과 모터 연동 후 동작 확인<br>* ADC 값 여러 개 받는 부분 => 인터럽트 or DMA<br>* J-Link 및 프로젝트 디버깅 문제<br><img src="https://user-images.githubusercontent.com/78337522/211183791-835c1ab4-cfcf-41ad-9358-dd68db4c0f40.jpg" width="250" height="350"/><img src="https://user-images.githubusercontent.com/78337522/211183793-4459c4e9-e01e-46f4-ac74-4b9ea3bb4d49.jpg" width="250" height="350"/><br>판 구현 팀 : 구조 설계 및 센서 배치<br><img src="https://user-images.githubusercontent.com/78337522/211183885-0d46cec2-335f-4626-b98e-fb7e14692994.JPG" width="400" height="550"/><br> |
|      22.12.02     | 코드 구현 팀 : 모터 및 ADC 동작 확인<br>- ADC channel 2개 => NVIC(인터럽트)로 됐는데 다시 안 됨<br>- 모터 코드 구조 수정 => 인터럽트 enable이 안 됨<br>- 다시 좀 더 찾아보기로 함.. |
| 22.12.06-22.12.07 | 코드 구현 팀 : 오류 파티 + 역행하는 중<br>- 갑자기 ADC 1개도 안 됨(ResetCalibration 부분) => 프로젝트 새로 생성해서 해결<br>- 블루투스 연결만 되고, 시리얼 통신 안 됨 (이유 알 수 없음)<br>- 모터 코드는 다시 원래대로 수정하기로 함|
| 22.12.06-22.12.09 | 판 구현 팀 : 판 기본 틀 완성, 세부적인 부분은 계속 수정할 예정|
|      22.12.09     | 코드 구현 팀 : 블루투스 여전히 안 되고, 프로젝트 디버깅 문제는 해결<br>- 모터 코드 구조 다시 복구 완료<br>- 진동 센서는 다시 값 두 개 이상 받기 목표로 돌아감<br>- 참고 레포 공유<br>- 다른 블루투스로 시도해봤는데 똑같이 안 돼서 다른 팀원들 모여서 다시 해 보기로 함|
|      22.12.13     | 기적 발생, 갑자기 모든 게 다시 잘 되기 시작 (수상함)<br>- 블루투스 통신 해결, 값 보내기도 성공 (값 보낼 때 int는 변환해야 함)<br>- 판 모터와 연결해서 동작 확인 => 많이 안 올라가져서 수정 필요<br>- ADC는 DMA로 해결, 조도+진동 센서를 각각 다 써야 해서 DMA 사용<br>=> 인터럽트 핸들러에서 값 찍을 때 if문 사용 <br><img src="https://user-images.githubusercontent.com/78337522/211183719-4a28b422-eb8a-4b64-984b-88cd6ea3027b.jpg" width="250" height="250"/><img src="https://user-images.githubusercontent.com/78337522/211183725-7dcaf3ab-e066-4f6a-b42d-40cb2f79d09b.jpg" width="250" height="250"/>|
|      22.12.19     | 판 선 정리<br>- LED가 잘 안 됨<br>- DC 모터 동작 확인... 등등<br><img src="https://user-images.githubusercontent.com/78337522/211183430-1e7958ec-f926-4c79-a5e1-37a967a27868.jpg" width="250" height="250"/><img src="https://user-images.githubusercontent.com/78337522/211183436-3457126a-9e1f-4400-b9c8-5c05a1556151.jpg" width="250" height="250"/><br><img src="https://user-images.githubusercontent.com/78337522/211183432-ab69ea2a-89a8-4324-a81c-64af229fed3e.jpg" width="250" height="250"/><img src="https://user-images.githubusercontent.com/78337522/211183605-afd92ce0-6e81-48b6-812c-94dc7b2c19df.png" width="250" height="250"/><br> |
|      22.12.20     | **최종!**<br>- DC 모터 작동 안 돼서 변경<br>- 서보모터 채널 사용 시 hardfault error 뜸 => 꼭 필요한 부분만 남김<br>- LED 포트 변경해서 성공<br>- 블루투스 점수 수신, 게임 종료 기능 추가<br>- 조도센서 값과 게임 종료 연결(그림자 생기면 안 됨)<br><img src="https://user-images.githubusercontent.com/78337522/211184205-6f119314-3816-4282-be08-98c19300cbd4.jpg" width="450" height="250"/><br><img src="https://user-images.githubusercontent.com/78337522/211184208-aac93216-b4bd-4f9d-9cd0-fe4edb998dcf.jpg" width="450" height="250"/><br><img src="https://user-images.githubusercontent.com/78337522/211184209-6e1c578c-6eb4-4d19-8db7-d0e1ba9b3613.jpg" width="450" height="250"/><br><img src="https://user-images.githubusercontent.com/78337522/211184213-75f58212-b569-4546-a564-37451773dadf.jpg" width="450" height="250"/><br><img src="https://user-images.githubusercontent.com/78337522/211184211-c0bbe9f8-0d04-477d-b815-a2df0834a34f.jpg" width="450" height="250"/><br><img src="https://user-images.githubusercontent.com/78337522/211184215-1999f513-b480-4966-8d8e-76331aa08595.jpg" width="450" height="250"/><br><img src="https://user-images.githubusercontent.com/78337522/211184214-7c6f81f9-2ae4-47b1-8fdb-142b3e839ee2.jpg" width="450" height="250"/><br><img src="https://user-images.githubusercontent.com/78337522/211184207-2c0e2a3a-c3d7-4bdc-a508-2ef82c1e90cc.jpg" width="450" height="250"/> |


<br>

***
<br>

<p align="center" style="font-weight: bold; font-size: 150%;">끝!</p>
<p align="center"><img src="https://user-images.githubusercontent.com/78337522/211184217-bed483a1-86fa-458c-8043-fb45cc59dadf.jpg" width="350" height="450"/><img src="https://user-images.githubusercontent.com/78337522/211184210-dbc1495f-2377-435e-9da3-69434674df18.jpg" width="260" height="450"/></p>
<p align="center"><img src="https://user-images.githubusercontent.com/78337522/211184216-a45758dc-4f68-44d6-9b33-4fbac4749686.jpg" width="450" height="250"/></p>
