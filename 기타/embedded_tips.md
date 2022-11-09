## 임베디드 시스템 설계 및 실험 003분반 6조

1. 레지스터 선언 및 설정
   - 각 레지스터의 base 주소에 offset을 더한 주소를 구하여 선언한다.
   - 사용할 GPIO Port를 Enable 상태로 초기화하는데, 이때 RCC_APB2ENR에서 4bit씩 끊어서 각 비트에 해당하는 port를 1로 만들고 16진수로 적어준다.
   - GPIO Port 설정 레지스터는 CRL, CRH 두 개가 있는데, CRL은 0~7, CRH는 8~15이다.
     - 하나로 8핀을 설정하므로, 16핀을 설정하려면 두 개가 있어야 한다.
     - 0x44444444 => 0100 0100 0100 0100 0100 0100 0100 0100(2) 는 Reset 값이다.
     - ex) 사용하려는 핀이 PB8이면 GPIOB_CRH 레지스터를 사용한다.




> 참고
> 
- https://munshark.tistory.com/entry/ARM데이터시트GPIO레지스터-GPIOxCRLGPIOxCRH
- http://realsys.co.kr/data/arm/2.%EC%B6%9C%EB%A0%A5%ED%8F%AC%ED%8A%B8%20%EC%86%8D%EB%8F%84%20%EB%86%92%EC%9D%B4%EA%B8%B0.pdf
- https://github.com/heyask/PNU-embedded-system/tree/master/3_week