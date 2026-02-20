<div align="center">

<h1>C-Chess-Engine</h1>

<p>
순수 C로 구현된 고전 체스 엔진
</p>

<p>
<img src="https://img.shields.io/badge/Language-C-blue.svg" />
<img src="https://img.shields.io/badge/Status-Development-orange.svg" />
<img src="https://img.shields.io/badge/Type-Chess%20Engine-black.svg" />
</p>

</div>

<hr>

<h2>프로젝트 소개</h2>

<p>
C-Chess-Engine은 순수 C 언어로 작성된
경량화 및 모듈형 체스 엔진입니다.
</p>

<p>
이 프로젝트는 이동 생성, 평가 함수,
트리 탐색을 포함한 고전적인 탐색 기반 체스 엔진의
핵심 구성 요소를 구현하는 데 초점을 두고 있습니다.
</p>

<hr>

<h2>핵심 구조</h2>

<table>
<tr>
<th align="left">구성 요소</th>
<th align="left">설명</th>
</tr>

<tr>
<td><b>보드 표현</b></td>
<td>
8x8 보드 상태를 유지하며,
기물 인코딩과 턴 관리를 담당합니다.
</td>
</tr>

<tr>
<td><b>이동 생성</b></td>
<td>
의사 합법 수를 생성하고,
킹이 노출되는 불법 수를 필터링합니다.
</td>
</tr>

<tr>
<td><b>탐색</b></td>
<td>
Alpha-Beta 가지치기가 적용된
깊이 제한 Minimax 알고리즘을 구현합니다.
</td>
</tr>

<tr>
<td><b>평가</b></td>
<td>
기물 가치 기반 점수 계산과
선택적 위치 휴리스틱을 포함합니다.
</td>
</tr>

</table>

<hr>

<h2>주요 기능</h2>

<ul>
<li>표준 8x8 체스 보드 구현</li>
<li>합법 수 검증</li>
<li>기물 가치 기반 평가 시스템</li>
<li>Minimax 탐색 알고리즘</li>
<li>Alpha-Beta 가지치기 최적화</li>
<li>모듈형 C 코드 구조</li>
<li>명령줄 인터페이스</li>
</ul>

<hr>

<h2>빌드 및 실행</h2>

<h3>요구 사항</h3>

<ul>
<li>GCC 또는 Clang 컴파일러</li>
</ul>

<h3>컴파일</h3>

<pre>
gcc -o chess main.c Board.c Engine.c
</pre>

<h3>실행</h3>

<pre>
./chess
</pre>

<hr>

<h2>엔진 동작 과정</h2>

<ol>
<li>보드 상태 초기화</li>
<li>합법 수 생성</li>
<li>후보 수에 대한 포지션 평가</li>
<li>Minimax + Alpha-Beta를 이용한 탐색</li>
<li>최적의 수 선택</li>
</ol>

<hr>

<h2>평가 모델</h2>

<table>
<tr>
<th align="left">기물</th>
<th align="right">가치</th>
</tr>

<tr>
<td>폰</td>
<td align="right">100</td>
</tr>

<tr>
<td>나이트</td>
<td align="right">320</td>
</tr>

<tr>
<td>비숍</td>
<td align="right">334</td>
</tr>

<tr>
<td>룩</td>
<td align="right">500</td>
</tr>

<tr>
<td>퀸</td>
<td align="right">900</td>
</tr>

<tr>
<td>킹</td>
<td align="right">20000</td>
</tr>

</table>

<hr>

<h2>향후 개선 계획</h2>

<ul>
<li>UCI 프로토콜 지원</li>
<li>트랜스포지션 테이블 (Zobrist 해싱)</li>
<li>반복 심화 탐색</li>
<li>이동 정렬 휴리스틱</li>
<li>퀘이슨스 탐색</li>
<li>오프닝 북 연동</li>
<li>엔드게임 테이블베이스</li>
</ul>

<hr>

<h2>프로젝트 구조</h2>

<pre>
C-Chess-Engine
│
├── main.c
├── board.c / board.h
├── move.c / move.h
├── search.c / search.h
├── evaluate.c / evaluate.h
└── README.md
</pre>

<hr>

<div align="center">

<h3>교육용 체스 엔진 프로젝트</h3>

<p>
C 언어에서 고전적인 AI 탐색 기법을
이해하기 위해 설계되었습니다.
</p>

</div>
