<div align="center">
  <h1>C-Chess-Engine — Documentation</h1>
  <p> C-Chess-Engine 프로젝트를 위한 기술 문서입니다.  
  본 문서는 아키텍처, 알고리즘, API(상위 수준), 테스트 방법을 설명합니다.</p>
  <hr>
</div>

<h2 id="toc">목차</h2>
<ul>
  <li><a href="#overview">개요</a></li>
  <li><a href="#board-representation">보드 표현</a></li>
  <li><a href="#move-generation">이동 생성</a></li>
  <li><a href="#search-algorithm">탐색 알고리즘</a></li>
  <li><a href="#evaluation-function">평가 함수</a></li>
  <li><a href="#engine-flow">엔진 흐름 및 API</a></li>
  <li><a href="#performance">성능 고려사항</a></li>
  <li><a href="#testing-debugging">테스트 및 디버깅</a></li>
  <li><a href="#extensions">확장 기능 및 고급 기능</a></li>
  <li><a href="#examples">예제 및 의사코드</a></li>
  <li><a href="#glossary">용어 정리</a></li>
</ul>

<hr>

<h2 id="overview">개요</h2>
<p>
본 문서는 C-Chess-Engine의 내부 설계와 예상 동작을 설명합니다.
board.c / move.c / search.c / evaluate.c 파일과 매핑될 수 있도록 작성되었으며,
리팩토링 시 명세서로도 활용할 수 있습니다.
</p>

<hr>

<h2 id="board-representation">보드 표현</h2>

<h3>권장 내부 모델</h3>
<p>단순성 또는 성능 목표에 따라 다음 중 하나를 선택할 수 있습니다.</p>
<ul>
  <li><b>64칸 배열 (0..63):</b> 구현이 단순하며 학습용으로 적합합니다.</li>
  <li><b>8x8 2차원 배열:</b> 가독성이 좋고 직관적입니다.</li>
  <li><b>비트보드:</b> 고급 방식으로, 비트 연산을 활용하여 매우 빠른 이동 생성을 지원합니다.</li>
</ul>

<h3>기물 인코딩 예시</h3>
<p>작은 정수 값을 사용하며, enum을 사용하여 구분합니다.</p>
<table>
  <tr><th>기물</th><th>코드</th></tr>
  <tr><td>폰</td><td>1</td></tr>
  <tr><td>나이트</td><td>2</td></tr>
  <tr><td>비숍</td><td>3</td></tr>
  <tr><td>룩</td><td>4</td></tr>
  <tr><td>퀸</td><td>5</td></tr>
  <tr><td>킹</td><td>6</td></tr>
</table>
<p>백 기물은 1-6, 흑 기물은 7-12, 빈 칸은 0으로 표현합니다.</p>

<hr>

<h2 id="move-generation">이동 생성</h2>

<h3>2단계 접근 방식</h3>
<ol>
  <li><b>의사 합법 수 생성</b>
    <ul>
      <li>체크 여부를 무시하고 가능한 모든 이동을 생성합니다.</li>
      <li>슬라이딩 기물 이동, 나이트 점프, 폰 전진/캡처, 승격을 포함합니다.</li>
      <li>특수 규칙: 캐슬링, 앙파상 포함 (조건 검사 필요).</li>
    </ul>
  </li>
  <li><b>합법 수 필터링</b>
    <ul>
      <li>이동을 적용(make)한 뒤 킹이 체크 상태인지 검사하고, 체크 상태라면 이후 되돌립니다(unmake).</li>
      <li>킹이 체크에 노출되는 수는 제거합니다.</li>
    </ul>
  </li>
</ol>

<h3>구현 팁</h3>
<ul>
  <li>전체 보드를 복사하지 않고 incremental make/unmake 방식을 사용합니다.</li>
  <li>캐슬링 권리와 앙파상 칸을 Position 구조체에 포함시킵니다.</li>
  <li>슬라이딩 기물은 방향 벡터를 따라 진행하며 첫 번째 차단 기물에서 중단합니다.</li>
</ul>

<hr>

<h2 id="search-algorithm">탐색 알고리즘</h2>

<h3>핵심: Alpha-Beta 기반 Minimax</h3>
<p>깊이 제한 minimax가 기본이며, Alpha-Beta 가지치기를 통해 탐색 노드를 크게 줄입니다.</p>

<h3>탐색 개선 로드맵</h3>
<ul>
  <li>반복 심화 탐색</li>
  <li>이동 정렬 휴리스틱 (캡처 우선, MVV-LVA, killer move 등)</li>
  <li>퀘이슨스 탐색 (Horizon Effect 완화)</li>
  <li>Null-move pruning</li>
  <li>트랜스포지션 테이블 (Zobrist 해싱)</li>
</ul>

<hr>

<h2 id="evaluation-function">평가 함수</h2>

<h3>기물 가치 기반</h3>
<table>
  <tr><th>기물</th><th>가치</th></tr>
  <tr><td>폰</td><td align="right">100</td></tr>
  <tr><td>나이트</td><td align="right">320</td></tr>
  <tr><td>비숍</td><td align="right">334</td></tr>
  <tr><td>룩</td><td align="right">500</td></tr>
  <tr><td>퀸</td><td align="right">900</td></tr>
  <tr><td>킹</td><td align="right">20000</td></tr>
</table>

<h3>위치적 요소 (선택)</h3>
<ul>
  <li>Piece-square 테이블</li>
  <li>폰 구조 (고립폰, 이중폰 등)</li>
  <li>킹 안전성</li>
  <li>기동성</li>
  <li>중앙 장악</li>
</ul>

<hr>

<h2 id="engine-flow">엔진 흐름 및 API (상위 수준)</h2>

<h3>권장 공개 함수 예시</h3>

<details>
<summary>Position / Board API</summary>
<pre><code>void init_position(Position *pos);
void load_fen(Position *pos, const char *fen);
void print_board(const Position *pos);
</code></pre>
</details>

<details>
<summary>이동 생성 API</summary>
<pre><code>int generate_moves(const Position *pos, MoveList *moves);
void make_move(Position *pos, Move m);
void unmake_move(Position *pos, Move m);
</code></pre>
</details>

<details>
<summary>탐색 API</summary>
<pre><code>Move search_best_move(Position *pos, int depth, SearchInfo *info);
int alphabeta(Position *pos, int depth, int alpha, int beta, SearchInfo *info);
</code></pre>
</details>

<details>
<summary>평가 API</summary>
<pre><code>int evaluate(const Position *pos);
</code></pre>
</details>

<hr>

<h2 id="performance">성능 고려사항</h2>

<ul>
  <li>탐색 루프에서 전체 보드 복사를 피합니다.</li>
  <li>트랜스포지션 테이블을 활용합니다.</li>
  <li>효율적인 이동 정렬을 구현합니다.</li>
  <li>고도 최적화 시 비트보드를 고려합니다.</li>
  <li>프로파일링 후 병목 지점을 우선 최적화합니다.</li>
</ul>

<hr>

<h2 id="testing-debugging">테스트 및 디버깅</h2>

<h3>작성 권장 테스트</h3>
<ul>
  <li>기물별 이동 생성 테스트</li>
  <li>특수 규칙 테스트 (캐슬링, 앙파상, 승격)</li>
  <li>체크/체크메이트/스테일메이트 판별</li>
  <li>make/unmake 일관성 테스트</li>
  <li>기준 포지션 평가 검증</li>
</ul>

<h3>디버깅 팁</h3>
<ul>
  <li>노드별 FEN 및 이동 목록 출력</li>
  <li>고정 깊이 탐색 비교</li>
  <li>Zobrist 키 로그 출력</li>
</ul>

<hr>

<h2 id="extensions">확장 기능 및 고급 기능</h2>

<details>
<summary>UCI 프로토콜</summary>
<p>UCI를 구현하면 GUI와 연동할 수 있습니다. 주요 명령: uci, isready, position, go, stop, quit</p>
</details>

<details>
<summary>트랜스포지션 테이블 & Zobrist</summary>
<p>64비트 Zobrist 키를 사용하여 포지션을 해싱합니다.</p>
</details>

<details>
<summary>퀘이슨스 탐색</summary>
<p>캡처 수열을 확장 탐색하여 Horizon Effect를 방지합니다.</p>
</details>

<hr>

<h2 id="examples">예제 및 의사코드</h2>

<h3>Alpha-Beta (negamax 스타일)</h3>
<pre><code>int negamax(Position *pos, int depth, int alpha, int beta) {
  if (depth == 0) return evaluate(pos);

  MoveList moves;
  generate_moves(pos, &moves);

  for each move in moves {
    make_move(pos, move);
    int score = -negamax(pos, depth-1, -beta, -alpha);
    unmake_move(pos, move);

    if (score >= beta) return beta;
    if (score > alpha) alpha = score;
  }
  return alpha;
}
</code></pre>

<h3>컴파일 및 실행 예시</h3>
<pre><code>gcc -o chess main.c board.c move.c search.c evaluate.c
./chess
</code></pre>

<hr>

<h2 id="glossary">용어 정리</h2>
<ul>
  <li><b>FEN:</b> 체스 포지션을 문자열로 표현하는 표기법</li>
  <li><b>Zobrist 해싱:</b> 포지션 고유 키 생성을 위한 해시 기법</li>
  <li><b>Quiescence:</b> 불안정한 캡처 상황을 해결하기 위한 확장 탐색</li>
  <li><b>UCI:</b> 체스 엔진과 GUI 간 통신 프로토콜</li>
</ul>

<hr>

<div align="center">
  <p>
    필요하다면 다음 단계로:
    <ul style="display:inline-block; text-align:left;">
      <li>실제 소스 코드에 맞춘 함수 단위 API 문서 생성</li>
      <li>main.c에 붙여넣을 수 있는 UCI 기본 코드 생성</li>
      <li>출력 가능한 PDF 문서 제작</li>
    </ul>
  </p>
</div>
