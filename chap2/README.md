# 실행결과

- outline control

https://www.youtube.com/shorts/HcgaRHAP9yM

- inline control

https://www.youtube.com/shorts/mrX-HJB3viI

# vision.cpp

영상처리 라이브러리 설명

https://github.com/lsy0727/jetson_linetracer/blob/main/chap1/README.md

# main.cpp

## 코드설명

https://github.com/lsy0727/jetson_linetracer/blob/337fb46e7de0354ca9e719558d5cbafdbc0647ea/chap2/main.cpp#L55

모터 구동 on/off 트리거

https://github.com/lsy0727/jetson_linetracer/blob/583be6f2cfc1241c569b71cfcae80ae66183c2ec/chap2/main.cpp#L79-L85

키보드로부터 'q'입력시 모터 구동 트리거 false, 's'입력시 모터 구동 트리거 true

ctrl+c입력시 종료

https://github.com/lsy0727/jetson_linetracer/blob/a91797fc3ddfc7b53cbb3f234ffdc625190ca114/chap2/main.cpp#L57

https://github.com/lsy0727/jetson_linetracer/blob/2f078040f3728bdab3a0a250542da4c5e7241792/chap2/main.cpp#L87-L88

게인값에 따라 바퀴의 민감도 조정

게인값이 클수록 회전이 빠르고, 작을수록 회전이 느림

https://github.com/lsy0727/jetson_linetracer/blob/301b22bab0774ea643963c426204192f0b762872/chap2/main.cpp#L89-L90

트리거 true인 경우 객체에 따른 모터 제어

false인 경우 모터속도 0으로 제어

## 코드수정

https://github.com/lsy0727/jetson_linetracer/blob/2a7b77663f22ed96df5d09476e4b4bc0473ad82a/chap2/main.cpp#L59-L61

https://github.com/lsy0727/jetson_linetracer/blob/168a7855c93ecc1393c740738a6375cb75ee6775/chap2/main.cpp#L92-L98

https://github.com/lsy0727/jetson_linetracer/blob/ab1190fa935d3730bc8be381ad39fd38716593e1/chap2/main.cpp#L93

runtime_diff : 무한루프 내의 함수 호출 등의 시간을 측정한 변수

diff = (원하는 딜레이 시간) - (runtime_diff)

측정하는 부분에서 약간의 오차가 생기겠지만, 이전 코드보다 오차가 줄어들었음


# 블록도

![image](https://github.com/user-attachments/assets/31085376-6b1a-4136-8583-3f0b7a74a940)
