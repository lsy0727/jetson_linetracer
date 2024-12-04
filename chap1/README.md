# 실행결과

https://www.youtube.com/watch?v=y1GEPU2oyZY

# <vision.cpp>

## 설명
https://github.com/lsy0727/jetson_linetracer/blob/c17fe10008568a9095b55729a9b66723a896d8a7/chap1/vision.cpp#L6-L22

* preprocess()함수 : 영상 전처리 함수

line 14~15 : 영상의 밝기 보정 (평균 100으로)

line 16 : 임계값 너무 작으면 노이즈가 생기고, 높으면 라인이 사라짐. 너무높지만 않으면 130보다 좀더 높아도 괜찮음.


https://github.com/lsy0727/jetson_linetracer/blob/066bfa827bbd73ece02550065d6da75e55f1d83d/chap1/vision.cpp#L25-L55

* findObjects()함수 : 객체 검출 함수

현재 검출된 객체(라인)들과 이전 프레임에서의 메인라인의 무게중심 점의 차이를 이용해 메인라인을 업데이트함.

거리는 150이하인 경우로 설정

이유 : (100일 때는 회전시에 급격한 변화가 생기면 라인을 놓치는 경우가 생기고, 150이 넘으면 다른 라인을 메인라인으로 변경해버리는 경우가 생김)

line 45~48 :  현재 영상에서 검출된 객체 중 가장 거리가 짧고, 이전 객체와 거리가 150이하인경우 객체가 있다고 판단( -> min_index 업데이트 )

1. 메인 라인이 업데이트 되는 경우(min_index가 업데이트 된 경우) 검출된 메인 라인의 좌표를 tmp_pt에 저장하고, 다음 라인검출시 비교용도로 사용

2. 객체가 없는 경우는 업데이트되지 않아, 사라지기 직전 프레임의 위치를 기준으로 찾음.


https://github.com/lsy0727/jetson_linetracer/blob/52029a6928be30dbb33f1d82258b6eeb6368e3d7/chap1/vision.cpp#L58-L76

* drawObjects()함수 : 객체와 기준점을 표시해주는 함수

tmp_pt : 메인라인의 무게중심 좌표가 저장되어있음

메인라인에는 빨간색 바운딩박스와 점을 찍고, 나머지 라인에는 파란색 바운딩박스와 점을 찍음.


https://github.com/lsy0727/jetson_linetracer/blob/fb40dde10008374f52f945060cc801049d7f6a36/chap1/vision.cpp#L79-L81

* getError()함수 : 위치오차를 계산해주는 함수

error = (로봇 중앙 x좌표) - (메인라인 중앙 x좌표)

(-)인 경우 : 라인이 오른쪽에 있음

(+)인 경우 : 라인이 왼쪽에 있음


# <main.cpp>

## 설명

https://github.com/lsy0727/jetson_linetracer/blob/82b25fbad80dfb97adc4edf82c793f36b098455e/chap1/main.cpp#L6-L10

영상을 불러옴

https://github.com/lsy0727/jetson_linetracer/blob/261123a09fc331b84b7809f7bebc751cf8640e5e/chap1/main.cpp#L26-L27

영상 전처리

https://github.com/lsy0727/jetson_linetracer/blob/c406770232e3fcdf98741f6cffa880b8882f5814/chap1/main.cpp#L30-L33

최초 실행시 기준 설정

https://github.com/lsy0727/jetson_linetracer/blob/80a19517260b8ffc05ea8293d8f007a4e2656ae4/chap1/main.cpp#L37-L40

최초 실행시의 기준 라인을 메인라인으로 생각했을 때

1. 객체(라인)을 모두 찾음
  
2. 메인라인은 빨간색 바운딩 박스와 중심점, 나머지 라인은 파란색 바운딩 박스와 중심점 표시

https://github.com/lsy0727/jetson_linetracer/blob/1619d478e0aeca9ee4d975dc6a2e0ee1ec71f651/chap1/main.cpp#L42

현 시점 error 계산

https://github.com/lsy0727/jetson_linetracer/blob/f2d57161b318946144a367d8b9375d62ffd5dc43/chap1/main.cpp#L26

https://github.com/lsy0727/jetson_linetracer/blob/4c6062c110504f4d9ca97dbf874aced3e1052934/chap1/main.cpp#L49-L51

line 26 : 반복문 시작시 시간계산 시작

line 49 : 시간계산 종료

line 50 : 걸린 시간 계산(sec)

https://github.com/lsy0727/jetson_linetracer/blob/b297930c770bec0a4020062ff96d38788a348cda/chap1/main.cpp#L54-L56

frame : 기본 영상

gray : 흑백 영상

thresh : 최종 결과 영상
