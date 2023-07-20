# GAME-Awesomenauts

<img width="100%" src="https://user-images.githubusercontent.com/79896868/254759951-aea1e6b1-2b97-43a8-952a-26836d1d2e19.png"/>
<h3>[DirectX 2D - Individual Project] 어썸너츠</h3>

<b>동영상(Client)</b>    : https://www.youtube.com/watch?v=jgt_VzlvIEE
<br/><b>동영상(Tool)</b>      : https://www.youtube.com/watch?v=BUzxY5WcQXY
<br/><b>장르</b>              : AOS
<br/><b>날짜</b>              : 2019.07.01 ~ 2019.08.01
<br/><b>인원</b>              : 1명
<br/><b>개발 환경</b>         : Visual Studio 2015 (x64)
<br/><b>개발 언어 및 도구</b>  : C++, MFC, DirectX9
<br/><b>Blog</b> : https://song-ift.tistory.com/42

<hr size="5">

* <b>맵 툴을 제작해 정적 오브젝트 및 충돌 타일 배치</b>

* <b>3D 공간의 좌표를 이용해 윈도우 좌표로 변환 후 스크롤 기능 구현</b>

* <b>Multi Thread를 통한 데이터 및 리소스를 로딩</b>

* <b>프레임 제한을 통한 동기화</b>

* <b>Object Polling을 이용한 투사체 최적화</b>

* <b>Y값 비교를 통해 객체 렌더링</b>
  - Y 소팅을 통해 우선순위를 정해 렌더링
  - 객체 뒤에 가려지는 플레이어를 표현하기 위해 AlphaBlend 적용

* <b>스킬창, 미니맵, 결과창, 알림 등 다양한 UI 구현</b>

* <b>비트 연산을 통한 키 입력 처리</b>
  - 별도의 키매니저에서 각 키에 대응하는 상수값들을 enum class로 선언하고 비트 연산을 통해 입력 처리, 모든 키에 대한 입력을 for 문 순회가 아닌 특정 키의 비트값을 비트 연산으로 비교해 입력했는지 확인
