# Miller-Rabin-Algorithm
## 문제
결정적(deterministic) 밀러라빈(Miller-Rabin) 알고리즘을 사용하여 길이가 최대 64 비트인 소수를 찾는  
프로그램을 구현한다. 베이스(base) 값 𝑎를 무작위로 선택하는 확률적 밀러라빈 알고리즘과는 달리 결정  
적 밀러라빈 알고리즘은 매우 작은 집합의 정해진 베이스 값만 검증한다. 그 이유는 𝑛 < 2^64이면 베이스  
값 𝑎를 집합 {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}에서만 검증하면 충분하다는 것이 밝혀졌기 때문이다.  
## 함수 구현
학생들이 구현할 함수의 프로토타입은 아래에 열거되어 있다. 각 함수에 대한 요구사항은 다음과 같다.  

• int miller_rabin(uint64_t n) – 64비트 음이 아닌 정수 n이 소수이면 PRIME을, 그렇지 않으  
면 COMPOSITE을 넘겨준다. n이 2^64 보다 작으므로 결정적 밀러라빈 알고리즘을 사용한다.  

• uint64_t mod_add(uint64_t a, uint64_t b, uint64_t m) – 𝑎 + 𝑏 mod 𝑚을 계산하여 넘  
겨준다. 𝑎와 𝑏가 각각 𝑚보다 작다는 가정하에서 𝑎와 𝑏의 합이 𝑚보다 크거나 같으면 결과에서  
𝑚을 빼줘야 한다. 이 경우 실제 계산은 𝑎 − (𝑚 − 𝑏)로 하는 것이 오버플로를 피할 수 있는 좋은  
방법이다. 또한 𝑎 + 𝑏 >= 𝑚을 검사하는 과정에서 오버플로가 발생할 수 있으므로 𝑏를 오른쪽으로  
넘겨 𝑎 >= 𝑚 − 𝑏를 검사하는 것이 오버플로를 피할 수 있는 현명한 방법이다.  

• uint64_t mod_sub(uint64_t a, uint64_t b, uint64_t m) – 𝑎−𝑏 mod 𝑚을 계산하여 넘겨  
준다. 𝑎와 𝑏가 각각 𝑚보다 작다는 가정하에서 𝑎가 𝑏보다 작으면 결과가 음이 되므로 𝑚을 더해준다.  
즉, 𝑎 + (𝑚 − 𝑏)으로 계산한다. 그렇지 않으면 원래대로 𝑎 − 𝑏로 계산한다.  

• uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t m) – 𝑎𝑏 mod 𝑚을 계산하여 넘겨  
준다. 오버플로가 발생할 수 있기 때문에 프로그래밍 언어가 제공하는 곱셈으로는 계산이 올바르지  
않을 수 있다. 앞에서 정의한 mod_add()가 오버플로를 고려했다는 점과 곱셈을 덧셈을 사용하여  
빠르게 계산할 수 있는 “double addition” 알고리즘을 사용하면 문제를 해결할 수 있다. 그 알고리  
즘은 다음과 같다.  

r = 0;  
while (b > 0) {  
  if (b & 1)  
    r = mod_add(r, a, m);  
  b = b >> 1;  
  a = mod_add(a, a, m);  
}  
return r;  

• uint64_t mod_pow(uint64_t a, uint64_t b, uint64_t m) – 𝑎^𝑏 mod 𝑚을계산하여넘겨준  
다. 오버플로가 발생할 수 있기 때문에 이 역시 프로그래밍 언어가 제공하는 지수함수로는 계산이  
올바르지 않을 수 있다. 앞에서 정의한 mod_mul()이 오버플로를 고려했다는 점과 지수연산을 곱  
셈을 사용하여 빠르게 계산할 수 있는 “square multiplication” 알고리즘을 사용하면 문제를 해결할  
수 있다. 그 알고리즘은 다음과 같다.  

r = 1;  
while (b > 0) {  
  if (b & 1)  
    r = mod_mul(r, a, m);  
  b = b >> 1;  
  a = mod_mul(a, a, m);  
}  
return r;  
## OpenMP 설치
OpenMP (Open Multi-Processing)는 다중코어 환경에서 병렬프로그래밍을 지원하는 API이다. 이 과제  
에서는 소수를 빨리 찾기 위해 OpenMP 라이브러리를 사용한다. 학생들이 과제를 수행하는데 OpenMP  
에 대한 지식이 꼭 필요한 것은 아니지만 검증 프로그램을 돌리기 위해서는 OpenMP를 설치해야 한다.  
• Linux 환경에서는 설치하지 않는다. 컴파일러에 내장되어 있다.  
• macOS 환경에서는 먼저 Homebrew를 설치해야 한다. 이미 Homebrew가 설치되어 있다면 이  
부분은 건너뛴다. Homebrew를 설치하려면 터미널을 열고 다음 명령어를 실행한다.  
% /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"  
Homebrew 설치가 완료되면 다음 명령어를 실행하여 OpenMP 라이브러리를 설치한다.  
% brew install libomp  
애플 실리콘이 탑재된 맥에서는 brew 명령어를 찾지 못하는 오류가 발생할 수 있다. brew 명령어가  
/usr/local/bin 밑에 있지 않고 /opt/homebrew/bin 밑에 있기 때문이다. /opt/homebrew/bin  
을 시스템 기본 실행 경로에 포함시켜야 한다. 헤더 파일과 라이브러리 파일 경로도 gcc 컴파일러  
환경변수에 추가해야 한다. 사용자 홈디렉토리로 이동하여 문자편집기로 .zshrc 파일을 열거나  
새로 생성한다. 아래 세 줄을 추가하고 저장한 후 터미널을 다시 시작한다.
export PATH=/opt/homebrew/bin:/opt/homebrew/sbin:$PATH
export LIBRARY_PATH=/opt/homebrew/lib:$LIBRARY_PATH
export C_INCLUDE_PATH=/opt/homebrew/include:$C_INCLUDE_PATH
골격 파일
구현이 필요한 골격파일 miller_rabin.skeleton.c와 함께 프로그램을 검증할 수 있는 test.c, 헤
더파일 miller_rabin.h, 그리고 Makefile을 제공한다. 이 가운데 test.c를 제외한 나머지 파일은
용도에 맞게 자유롭게 수정할 수 있다.
