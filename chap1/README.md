# 영상처리 라이브러리

## vision.cpp

### 설명

https://github.com/lsy0727/jetson_linetracer/blob/cd84c6d91a136ec3304e6f48c4f18abb1a66b83e/chap1/vision.cpp#L6-L22

preprocess()함수 : 영상 전처리 함수

https://github.com/lsy0727/jetson_linetracer/blob/c10f8092c896652d5c61a2662164b3eff3ca4788/chap1/vision.cpp#L25-L57

findObjects()함수 : 객체 검출 함수

https://github.com/lsy0727/jetson_linetracer/blob/539a22c043c9ed35942077eefe665d25f007fd9b/chap1/vision.cpp#L60-L78

drawObjects()함수 : 객체범위를 표시해주는 함수

https://github.com/lsy0727/jetson_linetracer/blob/40fa32b5a97dcdbc3ce62d765fc419cf18c0d639/chap1/vision.cpp#L81-L83

getError()함수 : 위치오차를 계산해주는 함수

(-)인 경우 : 라인이 오른쪽에 있음

(+)인 경우 : 라인이 왼쪽에 있음


## main.cpp

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

## 실행결과

https://www.youtube.com/watch?v=y1GEPU2oyZY
