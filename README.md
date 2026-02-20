<!DOCTYPE html>
<html lang="ko">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
  <title>C-Chess-Engine</title>
  <style>
    *, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }
    body {
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", "Noto Sans KR", Helvetica, Arial, sans-serif;
      background: #0d1117; color: #e6edf3; line-height: 1.7; padding: 40px 20px;
    }
    .container { max-width: 860px; margin: 0 auto; }
    .header { text-align: center; padding: 48px 0 40px; border-bottom: 1px solid #30363d; margin-bottom: 48px; }
    .header h1 { font-size: 2.4rem; font-weight: 700; color: #f0f6fc; margin-bottom: 12px; }
    .header p { color: #8b949e; font-size: 1.05rem; margin-bottom: 20px; }
    .badges { display: flex; gap: 8px; justify-content: center; flex-wrap: wrap; }
    .badge { display: inline-block; padding: 4px 12px; border-radius: 20px; font-size: 0.78rem; font-weight: 600; }
    .badge-blue   { background: #1f4a8a; color: #79b8ff; border: 1px solid #1f4a8a; }
    .badge-orange { background: #3d2200; color: #f0883e; border: 1px solid #3d2200; }
    .badge-dark   { background: #21262d; color: #c9d1d9; border: 1px solid #30363d; }
    h2 { font-size: 1.35rem; font-weight: 600; color: #f0f6fc; border-bottom: 1px solid #30363d; padding-bottom: 10px; margin: 40px 0 18px; }
    h3 { font-size: 1.05rem; font-weight: 600; color: #e6edf3; margin: 24px 0 10px; }
    p { color: #c9d1d9; margin-bottom: 12px; }
    table { width: 100%; border-collapse: collapse; margin: 16px 0 24px; font-size: 0.92rem; }
    thead th { background: #161b22; color: #8b949e; font-weight: 600; padding: 10px 14px; text-align: left; border-bottom: 1px solid #30363d; text-transform: uppercase; font-size: 0.78rem; }
    tbody td { padding: 10px 14px; border-bottom: 1px solid #21262d; color: #c9d1d9; vertical-align: top; }
    tbody tr:last-child td { border-bottom: none; }
    tbody tr:hover td { background: #161b22; }
    td strong { color: #e6edf3; }
    pre { background: #161b22; border: 1px solid #30363d; border-radius: 6px; padding: 18px 20px; overflow-x: auto; margin: 12px 0 20px; font-size: 0.88rem; }
    code { font-family: "SFMono-Regular", Consolas, monospace; color: #e6edf3; }
    p code, li code { background: #161b22; border: 1px solid #30363d; border-radius: 4px; padding: 2px 6px; font-size: 0.85em; color: #79b8ff; }
    ul, ol { padding-left: 24px; margin-bottom: 16px; }
    li { color: #c9d1d9; margin-bottom: 6px; }
    .mode-grid { display: grid; grid-template-columns: repeat(auto-fill, minmax(240px, 1fr)); gap: 14px; margin: 16px 0 24px; }
    .mode-card { background: #161b22; border: 1px solid #30363d; border-radius: 8px; padding: 18px; }
    .mode-card h4 { font-size: 0.95rem; font-weight: 600; color: #58a6ff; margin-bottom: 8px; }
    .mode-card p { font-size: 0.9rem; margin-bottom: 0; }
    .file-tree { background: #161b22; border: 1px solid #30363d; border-radius: 6px; padding: 18px 20px; font-family: "SFMono-Regular", Consolas, monospace; font-size: 0.88rem; color: #c9d1d9; line-height: 1.9; margin: 12px 0 20px; }
    .file-tree .dir { color: #79b8ff; font-weight: 600; }
    .file-tree .note { color: #8b949e; font-style: italic; }
    .roadmap { list-style: none; padding: 0; }
    .roadmap li { display: flex; align-items: flex-start; gap: 10px; padding: 8px 0; border-bottom: 1px solid #21262d; }
    .roadmap li:last-child { border-bottom: none; }
    .roadmap .dot { width: 8px; height: 8px; border-radius: 50%; background: #30363d; margin-top: 7px; flex-shrink: 0; }
    .footer { margin-top: 60px; padding-top: 24px; border-top: 1px solid #30363d; text-align: center; color: #8b949e; font-size: 0.88rem; }
    a { color: #58a6ff; text-decoration: none; }
    a:hover { text-decoration: underline; }
  </style>
</head>
<body>
<div class="container">

  <div class="header">
    <h1>♟ C-Chess-Engine</h1>
    <p>순수 C 언어로 작성된 경량 모듈형 체스 엔진</p>
    <div class="badges">
      <span class="badge badge-blue">언어: C</span>
      <span class="badge badge-orange">상태: 개발 중</span>
      <span class="badge badge-dark">종류: 체스 엔진</span>
      <span class="badge badge-dark">플랫폼: Windows</span>
    </div>
  </div>

  <h2>프로젝트 소개</h2>
  <p>C-Chess-Engine은 외부 라이브러리 없이 순수 C 언어로 작성된 고전적인 탐색 기반 체스 엔진입니다. 이동 생성, 평가 함수, 트리 탐색 등 체스 엔진의 핵심 구성 요소를 구현하며, 모듈형 구조로 가독성과 확장성을 갖추고 있습니다.</p>
  <p>C 언어에서 고전적인 AI 탐색 기법을 이해하기 위한 교육용 프로젝트로, 플레이어 대 플레이어, 플레이어 대 엔진, 분석 모드 세 가지 게임 모드를 지원합니다.</p>

  <h2>핵심 구조</h2>
  <table>
    <thead><tr><th>구성 요소</th><th>파일</th><th>설명</th></tr></thead>
    <tbody>
      <tr><td><strong>보드</strong></td><td><code>Board.c / Board.h</code></td><td>보드 상태 관리, 이동 유효성 검사, 합법 수 생성, 특수 규칙 처리</td></tr>
      <tr><td><strong>엔진</strong></td><td><code>Engine.c / Engine.h</code></td><td>Minimax 탐색, 알파베타 가지치기, 무브 오더링, 평가 함수</td></tr>
      <tr><td><strong>진입점</strong></td><td><code>main.c</code></td><td>게임 루프, 사용자 입력 처리, 모드 선택</td></tr>
    </tbody>
  </table>

  <h2>주요 기능</h2>
  <h3>보드 및 규칙</h3>
  <ul>
    <li>1차원 배열 보드 표현 (64칸, 인덱스 0 = a1)</li>
    <li>킹 안전 검증을 포함한 완전한 합법 수 생성</li>
    <li>앙파상, 캐슬링(킹사이드·퀸사이드), 폰 프로모션 지원</li>
    <li>체크메이트 및 스테일메이트 감지 (전수 탐색 방식)</li>
    <li>터미널 유니코드 기물 렌더링</li>
  </ul>
  <h3>탐색 엔진</h3>
  <ul>
    <li>알파베타 가지치기가 적용된 Minimax 알고리즘</li>
    <li>Board를 값으로 전달 — 별도의 undo 로직 불필요</li>
    <li>MVV-LVA 기반 무브 오더링 (캡처 수 우선 탐색)</li>
    <li>탐색 트리에 캐슬링 수 포함</li>
    <li>Windows API 기반 멀티스레드 루트 탐색 (<code>CreateThread</code>)</li>
  </ul>
  <h3>평가 함수</h3>
  <ul>
    <li>기물 가치 기반 재료 점수</li>
    <li>모든 기물 유형에 대한 위치 보너스 테이블 (Piece-Square Table)</li>
    <li>킹 안전도 평가: 폰 방패 보너스, 캐슬링 권한 보너스</li>
  </ul>

  <h2>게임 모드</h2>
  <div class="mode-grid">
    <div class="mode-card">
      <h4>1. 플레이어 대 플레이어</h4>
      <p>두 명의 플레이어가 대수 표기법으로 번갈아 수를 입력합니다.</p>
    </div>
    <div class="mode-card">
      <h4>2. 플레이어 대 엔진</h4>
      <p>AI와 대결합니다. 백 또는 흑을 선택할 수 있으며 엔진은 설정된 깊이로 탐색합니다.</p>
    </div>
    <div class="mode-card">
      <h4>3. 분석 모드</h4>
      <p>엔진이 현재 국면의 최선의 수를 제안합니다. 수를 적용하며 분석을 이어갈 수 있습니다.</p>
    </div>
  </div>

  <h2>기물 가치</h2>
  <table>
    <thead><tr><th>기물</th><th>가치</th></tr></thead>
    <tbody>
      <tr><td>폰</td><td>100</td></tr>
      <tr><td>나이트</td><td>320</td></tr>
      <tr><td>비숍</td><td>334</td></tr>
      <tr><td>룩</td><td>500</td></tr>
      <tr><td>퀸</td><td>900</td></tr>
      <tr><td>킹</td><td>20000</td></tr>
    </tbody>
  </table>

  <h2>빌드 및 실행</h2>
  <h3>요구 사항</h3>
  <ul>
    <li>Windows (Windows 스레드 API 사용)</li>
    <li>GCC 또는 MSVC 컴파일러</li>
  </ul>
  <h3>컴파일 (GCC)</h3>
  <pre><code>gcc -O2 -o chess main.c Board.c Engine.c</code></pre>
  <h3>실행</h3>
  <pre><code>./chess</code></pre>
  <h3>입력 형식</h3>
  <pre><code>e2e4       # 일반 이동 (출발칸 → 목적칸)
O-O        # 킹사이드 캐슬링
O-O-O      # 퀸사이드 캐슬링</code></pre>

  <h2>프로젝트 구조</h2>
  <div class="file-tree">
    <span class="dir">C-Chess-Engine/</span><br>
    &nbsp;&nbsp;├── main.c &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="note"># 게임 루프, 모드 선택, 입력 처리</span><br>
    &nbsp;&nbsp;├── Board.c &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="note"># 보드 상태, 이동 생성, 규칙 처리</span><br>
    &nbsp;&nbsp;├── Board.h<br>
    &nbsp;&nbsp;├── Engine.c &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="note"># Minimax 탐색, 평가 함수, 무브 오더링</span><br>
    &nbsp;&nbsp;├── Engine.h<br>
    &nbsp;&nbsp;├── README.md<br>
    &nbsp;&nbsp;└── DOCUMENTATION.md
  </div>

  <h2>향후 개선 계획</h2>
  <ul class="roadmap">
    <li><span class="dot"></span>Minimax 탐색 트리 내 캐슬링 적용 수정</li>
    <li><span class="dot"></span>UCI 프로토콜 지원 (Lichess 봇 등록용)</li>
    <li><span class="dot"></span>트랜스포지션 테이블 (Zobrist 해싱)</li>
    <li><span class="dot"></span>반복 심화 탐색 (Iterative Deepening)</li>
    <li><span class="dot"></span>퀴에이슨스 탐색 (Quiescence Search)</li>
    <li><span class="dot"></span>반복 수 감지 및 패널티</li>
    <li><span class="dot"></span>오프닝 북 연동</li>
    <li><span class="dot"></span>엔드게임 테이블베이스</li>
  </ul>

  <div class="footer">
    <p><a href="https://github.com/haamdong/C-Chess-Engine">github.com/haamdong/C-Chess-Engine</a> &nbsp;·&nbsp; 순수 C로 작성된 교육용 체스 엔진 프로젝트</p>
  </div>

</div>
</body>
</html>
