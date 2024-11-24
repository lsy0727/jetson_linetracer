# 영상처리 라이브러리

## 실행결과

https://www.youtube.com/watch?v=y1GEPU2oyZY

## <vision.cpp>

### 설명

https://github.com/lsy0727/jetson_linetracer/blob/cd84c6d91a136ec3304e6f48c4f18abb1a66b83e/chap1/vision.cpp#L6-L22

preprocess()함수 : 영상 전처리 함수

line 14~15 : 영상의 밝기 보정 (평균 100으로)

line 16 : 임계값 너무 작으면 노이즈가 생기고, 높으면 라인이 사라짐. 너무높지만 않으면 130보다 좀더 높아도 괜찮음.

https://github.com/lsy0727/jetson_linetracer/blob/518bdbc38410d976e7e0ab2a2254c8752fe51c04/chap1/vision.cpp#L25-L55

findObjects()함수 : 객체 검출 함수

현재 검출된 객체(라인)들과 이전 프레임에서의 메인라인의 무게중심 점의 차이를 이용해 메인라인을 업데이트함.

거리는 150이하인 경우로 설정

(100일 때는 회전시에 급격한 변화가 생기면 라인을 놓치는 경우가 생기고, 150이 넘으면 다른 라인을 메인라인으로 업데이트해버리는 경우가 생김)

1. 메인 라인이 업데이트 되는 경우(메인 라인이라고 인식한 객체가 존재하는 경우)

검출된 메인 라인의 좌표를 tmp_pt에 저장하고, 다음 라인검출시 비교용도로 사용

prev_pt : 

https://github.com/lsy0727/jetson_linetracer/blob/539a22c043c9ed35942077eefe665d25f007fd9b/chap1/vision.cpp#L60-L78

drawObjects()함수 : 객체범위를 표시해주는 함수

https://github.com/lsy0727/jetson_linetracer/blob/40fa32b5a97dcdbc3ce62d765fc419cf18c0d639/chap1/vision.cpp#L81-L83

getError()함수 : 위치오차를 계산해주는 함수

(-)인 경우 : 라인이 오른쪽에 있음

(+)인 경우 : 라인이 왼쪽에 있음


## <main.cpp>

### 설명

https://github.com/lsy0727/jetson_linetracer/blob/82b25fbad80dfb97adc4edf82c793f36b098455e/chap1/main.cpp#L6-L10

영상을 불러옴

https://github.com/lsy0727/jetson_linetracer/blob/261123a09fc331b84b7809f7bebc751cf8640e5e/chap1/main.cpp#L26-L27

영상 전처리

https://github.com/lsy0727/jetson_linetracer/blob/c406770232e3fcdf98741f6cffa880b8882f5814/chap1/main.cpp#L30-L33

최초 실행시 기준 설정

https://github.com/lsy0727/jetson_linetracer/blob/f5e2ad87f9b77e8ca2da1fdaf8de13d5fb2534df/chap1/main.cpp#L35-L38

최초 실행시의 기준 라인을 메인라인으로 생각했을 때

1. 객체(라인)을 모두 찾음
  
2. 메인라인은 빨간색 바운딩 박스와 중심점, 나머지 라인은 파란색 바운딩 박스와 중심점 표시

https://github.com/lsy0727/jetson_linetracer/blob/d10a9cc21ece733cbd4b873532d85b372977e961/chap1/main.cpp#L40

현 시점 error 계산

https://github.com/lsy0727/jetson_linetracer/blob/2d947b356bf090cd9afc0f4353637d248320d3ad/chap1/main.cpp#L24

https://github.com/lsy0727/jetson_linetracer/blob/2b5a30f8590fb0121b22cc7615241dde43cb77da/chap1/main.cpp#L45-L47

시간계산 후 시간과 error출력

https://github.com/lsy0727/jetson_linetracer/blob/000168e56e40655f6ae08b9b924d29e525f42a4e/chap1/main.cpp#L50-L52

frame : 기본 영상

gray : 흑백 영상

thresh : 최종 결과 영상
