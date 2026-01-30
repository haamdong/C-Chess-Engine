# C-Chess-Engine
순수 C언어만을 사용하여 구현한 CLI 기반 체스 엔진 프로젝트입니다. 

## 📌 프로젝트 개요
- **개발 기간**: 2026. 01. 30 ~ 
- **언어**: C (C99 이상)
- **인터페이스**: Command Line Interface (CLI)

## 🛠 설계 특징
- **효율적인 데이터 구조**: 8x8 2차원 논리를 64칸의 1차원 배열 인덱스로 매핑하여 연산 속도 및 메모리 효율을 높였습니다.
- **포인터 및 주소 연산**: 구조체 포인터를 활용한 상태 관리와 효율적인 데이터 전달을 지향합니다.
- **가독성 높은 코드**: 명확한 함수 명명 규칙을 준수하며, 외부 라이브러리 의존성을 최소화했습니다.

## 🗺 보드 인덱스 맵 (Index Mapping)
체스판의 좌표는 다음과 같이 1차원 배열 인덱스(0-63)로 관리됩니다.

  <table> <thead> <tr> <th align="center">Rank \ File</th> <th align="center">a</th><th align="center">b</th><th align="center">c</th><th align="center">d</th> <th align="center">e</th><th align="center">f</th><th align="center">g</th><th align="center">h</th> </tr> </thead> <tbody> <tr><td align="center"><b>8</b></td><td>56</td><td>57</td><td>58</td><td>59</td><td>60</td><td>61</td><td>62</td><td>63</td></tr> <tr><td align="center"><b>7</b></td><td>48</td><td>49</td><td>50</td><td>51</td><td>52</td><td>53</td><td>54</td><td>55</td></tr> <tr><td align="center"><b>6</b></td><td>40</td><td>41</td><td>42</td><td>43</td><td>44</td><td>45</td><td>46</td><td>47</td></tr> <tr><td align="center"><b>5</b></td><td>32</td><td>33</td><td>34</td><td>35</td><td>36</td><td>37</td><td>38</td><td>39</td></tr> <tr><td align="center"><b>4</b></td><td>24</td><td>25</td><td>26</td><td>27</td><td>28</td><td>29</td><td>30</td><td>31</td></tr> <tr><td align="center"><b>3</b></td><td>16</td><td>17</td><td>18</td><td>19</td><td>20</td><td>21</td><td>22</td><td>23</td></tr> <tr><td align="center"><b>2</b></td><td>8</td><td>9</td><td>10</td><td>11</td><td>12</td><td>13</td><td>14</td><td>15</td></tr> <tr><td align="center"><b>1</b></td><td>0</td><td>1</td><td>2</td><td>3</td><td>4</td><td>5</td><td>6</td><td>7</td></tr> </tbody> </table>


## 🚀 주요 기능
- [x] 체스판 초기화 및 출력 (CLI)
- [x] 대수 기보법(e2e4) 기반 입력 및 인덱스 변환
- [x] 기본적인 기물 이동 로직
- [ ] 각 기물별 이동 유효성 검사
- [ ] 체크/체크메이트 판정 로직
- [ ] 미니맥스(Minimax) 알고리즘 기반 AI
- [ ] 알파-베타 가지치기(Alpha-Beta Pruning)
- [ ] 앙파상(En Passant), 캐슬링(O-O, O-O-O) 등 특수 행마법 구현
- [ ] 오프닝 북 추가

## ⚙️ 실행 방법
bash
gcc -o chess main.c
./chess
