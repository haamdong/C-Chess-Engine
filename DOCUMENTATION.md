<div align="center">
  <h1>C-Chess-Engine — Documentation</h1>
  <p>Technical documentation for the C-Chess-Engine project.  
  This document describes architecture, algorithms, APIs (high-level), and testing tips.</p>
  <hr>
</div>

<h2 id="toc">Table of Contents</h2>
<ul>
  <li><a href="#overview">Overview</a></li>
  <li><a href="#board-representation">Board Representation</a></li>
  <li><a href="#move-generation">Move Generation</a></li>
  <li><a href="#search-algorithm">Search Algorithm</a></li>
  <li><a href="#evaluation-function">Evaluation Function</a></li>
  <li><a href="#engine-flow">Engine Flow & API</a></li>
  <li><a href="#performance">Performance Considerations</a></li>
  <li><a href="#testing-debugging">Testing & Debugging</a></li>
  <li><a href="#extensions">Extensions & Advanced Features</a></li>
  <li><a href="#examples">Examples & Pseudocode</a></li>
  <li><a href="#glossary">Glossary</a></li>
</ul>

<hr>

<h2 id="overview">Overview</h2>
<p>
This documentation explains the internal design and expected behavior of the C-Chess-Engine.
It intentionally describes concepts at a level that allows implementation mapping to existing source files
(board.c / move.c / search.c / evaluate.c) or to be used as a spec for refactoring.
</p>

<hr>

<h2 id="board-representation">Board Representation</h2>

<h3>Recommended internal models</h3>
<p>Choose one of the following depending on simplicity vs. performance:</p>
<ul>
  <li><b>64-element array (0..63):</b> simple, easy indexing; common for learning projects.</li>
  <li><b>8x8 2D array:</b> readable, intuitive mapping to ranks/files.</li>
  <li><b>Bitboards:</b> advanced—fast for move generation and bitwise ops, recommended when optimizing.</li>
</ul>

<h3>Piece encoding (example)</h3>
<p>Use small integer codes for pieces; sign represents color.</p>
<table>
  <tr><th>Piece</th><th>Code</th></tr>
  <tr><td>Pawn</td><td>1</td></tr>
  <tr><td>Knight</td><td>2</td></tr>
  <tr><td>Bishop</td><td>3</td></tr>
  <tr><td>Rook</td><td>4</td></tr>
  <tr><td>Queen</td><td>5</td></tr>
  <tr><td>King</td><td>6</td></tr>
</table>
<p>White pieces: positive values. Black pieces: negative values. Empty: 0.</p>

<hr>

<h2 id="move-generation">Move Generation</h2>

<h3>Two-stage approach</h3>
<ol>
  <li><b>Pseudo-legal move generation</b>
    <ul>
      <li>Generate all moves ignoring checks.</li>
      <li>Include sliding moves, knight jumps, pawn pushes/captures, promotions.</li>
      <li>Include special cases: castling and en passant (subject to rule checks).</li>
    </ul>
  </li>
  <li><b>Legal filtering</b>
    <ul>
      <li>Apply move (make), test if own king is in check, undo move (unmake).</li>
      <li>Reject moves that leave the king in check.</li>
    </ul>
  </li>
</ol>

<h3>Implementation tips</h3>
<ul>
  <li>Use incremental <i>make</i>/ <i>unmake</i> operations to avoid copying full board states.</li>
  <li>Keep track of castling rights and en-passant square in the position state.</li>
  <li>For sliding pieces, iterate along direction vectors and stop on first blocker.</li>
</ul>

<hr>

<h2 id="search-algorithm">Search Algorithm</h2>

<h3>Core: Minimax with Alpha-Beta</h3>
<p>Depth-limited minimax is the baseline. Alpha-Beta pruning reduces explored nodes significantly.</p>

<h3>Search enhancements (recommended roadmap)</h3>
<ul>
  <li>Iterative deepening (supports time control and move ordering)</li>
  <li>Move ordering heuristics (captures first, MVV-LVA, killer moves)</li>
  <li>Quiescence search (resolve horizon effect for captures)</li>
  <li>Null-move pruning (aggressive pruning technique)</li>
  <li>Transposition table (Zobrist hashing)</li>
</ul>

<hr>

<h2 id="evaluation-function">Evaluation Function</h2>

<h3>Material baseline</h3>
<table>
  <tr><th>Piece</th><th>Value</th></tr>
  <tr><td>Pawn</td><td align="right">100</td></tr>
  <tr><td>Knight</td><td align="right">320</td></tr>
  <tr><td>Bishop</td><td align="right">334</td></tr>
  <tr><td>Rook</td><td align="right">500</td></tr>
  <tr><td>Queen</td><td align="right">900</td></tr>
  <tr><td>King</td><td align="right">20000</td></tr>
</table>

<h3>Positional components (optional)</h3>
<ul>
  <li>Piece-square tables</li>
  <li>Pawn structure (isolated/doubled/backward pawns)</li>
  <li>King safety (pawn shield, open files)</li>
  <li>Mobility (number of legal moves)</li>
  <li>Control of center</li>
</ul>

<hr>

<h2 id="engine-flow">Engine Flow & API (high-level)</h2>

<h3>Suggested public-facing functions (module-level)</h3>
<details>
<summary>Position / Board API</summary>
<pre><code>void init_position(Position *pos);
void load_fen(Position *pos, const char *fen);
void print_board(const Position *pos);
</code></pre>
</details>

<details>
<summary>Move generation API</summary>
<pre><code>int generate_moves(const Position *pos, MoveList *moves);
void make_move(Position *pos, Move m);
void unmake_move(Position *pos, Move m);
</code></pre>
</details>

<details>
<summary>Search API</summary>
<pre><code>Move search_best_move(Position *pos, int depth, SearchInfo *info);
int alphabeta(Position *pos, int depth, int alpha, int beta, SearchInfo *info);
</code></pre>
</details>

<details>
<summary>Evaluation API</summary>
<pre><code>int evaluate(const Position *pos);
</code></pre>
</details>

<p>These are high-level prototypes. Adapt argument types to your codebase (e.g., MoveList, Position structs).</p>

<hr>

<h2 id="performance">Performance Considerations</h2>

<ul>
  <li>Avoid full board copies in tight loops—use incremental make/unmake.</li>
  <li>Use transposition tables to reuse evaluated positions.</li>
  <li>Implement fast move ordering to maximize alpha-beta prunes.</li>
  <li>When optimizing heavily, consider switching to bitboards for move generation.</li>
  <li>Profile hotspots (search, move generation, evaluation) and optimize the most expensive routines first.</li>
</ul>

<hr>

<h2 id="testing-debugging">Testing & Debugging</h2>

<h3>Unit tests to create</h3>
<ul>
  <li>Per-piece move generation tests (e.g., knight moves in corner)</li>
  <li>Special moves: castling rights, en passant, promotion</li>
  <li>Check / checkmate / stalemate detection</li>
  <li>Make/unmake consistency tests (apply then undo should restore state)</li>
  <li>Static evaluation sanity checks on known positions</li>
</ul>

<h3>Debugging tips</h3>
<ul>
  <li>Print FEN and move list at each node when debugging search logic.</li>
  <li>Use small fixed-depth searches to compare expected vs. engine moves.</li>
  <li>Log Zobrist keys (if used) to validate transposition hits/misses.</li>
</ul>

<hr>

<h2 id="extensions">Extensions & Advanced Features</h2>

<details>
<summary>UCI Protocol</summary>
<p>Implementing UCI makes the engine compatible with GUIs (e.g., Arena, CuteChess). Key commands to support: <code>uci</code>, <code>isready</code>, <code>position</code>, <code>go</code>, <code>stop</code>, <code>quit</code>.</p>
</details>

<details>
<summary>Transposition Table & Zobrist</summary>
<p>Use 64-bit Zobrist keys to hash positions. Store best move, depth, score, and node type (exact/lower/upper).</p>
</details>

<details>
<summary>Quiescence Search</summary>
<p>Extend search leaf nodes to evaluate capture sequences and avoid horizon effects.</p>
</details>

<hr>

<h2 id="examples">Examples & Pseudocode</h2>

<h3>Alpha-Beta (negamax style)</h3>
<pre><code>int negamax(Position *pos, int depth, int alpha, int beta) {
  if (depth == 0) return evaluate(pos);

  MoveList moves;
  generate_moves(pos, &moves);

  for each move in moves {
    make_move(pos, move);
    int score = -negamax(pos, depth-1, -beta, -alpha);
    unmake_move(pos, move);

    if (score >= beta) return beta;      // beta cutoff
    if (score > alpha) alpha = score;    // best so far
  }
  return alpha;
}
</code></pre>

<h3>Compile & Run (example)</h3>
<pre><code>gcc -o chess main.c board.c move.c search.c evaluate.c
./chess
</code></pre>

<hr>

<h2 id="glossary">Glossary</h2>
<ul>
  <li><b>FEN:</b> Forsyth–Edwards Notation — string that describes a position.</li>
  <li><b>Zobrist hashing:</b> Randomized hashing for unique position keys.</li>
  <li><b>Quiescence:</b> Search extension to resolve volatile captures.</li>
  <li><b>UCI:</b> Universal Chess Interface — protocol for engines and GUIs.</li>
</ul>

<hr>

<div align="center">
  <p>If you want, I can next:
    <ul style="display:inline-block; text-align:left;">
      <li>Generate a function-level API document matching your actual source files</li>
      <li>Create UCI skeleton code to paste into main.c</li>
      <li>Produce a printable PDF of this documentation</li>
    </ul>
  </p>
</div>
