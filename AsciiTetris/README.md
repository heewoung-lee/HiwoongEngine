# ASCII Tetris

[← HiwoongEngine 소개](../README.md)

HiwoongEngine의 Scene, Component, 부모·자식 Transform, 문자 렌더링을 조합해 만든 콘솔 테트리스입니다. 7종의 조각을 움직이고 회전시켜 줄을 지우며, 점수에 따라 레벨과 자동 낙하 속도가 달라집니다.

엔진이 입력과 객체 생명주기, 화면 출력을 처리하고, 게임은 조각의 동작·보드 점유·생성·상태 표시를 나눠 담당합니다. 공통 엔진 위에 실제 게임 규칙을 연결한 2D 적용 사례입니다.

## 플레이 데모

<table>
  <tr>
    <td align="center"><img width="270" alt="조각 이동과 회전, 다음 조각 표시" src="https://github.com/user-attachments/assets/e3aac143-c780-4849-aec3-6f6c003a9c8e"><br><sub>이동 · 회전 · 다음 블록 UI</sub></td>
    <td align="center"><img width="270" alt="보드 점유 검사와 게임 오버 전환" src="https://github.com/user-attachments/assets/1b7be359-cb4a-4c1d-8970-8a63505e4a90"><br><sub>보드 점유 검사와 GameOver 전환</sub></td>
    <td align="center"><img width="270" alt="상태 유지와 레벨별 낙하 속도" src="https://github.com/user-attachments/assets/cdb4ea95-407a-477e-874d-57338bec7e0d"><br><sub>상태 유지와 레벨별 낙하 속도</sub></td>
  </tr>
</table>

## 조작과 실행

| 키 | 동작 |
|---|---|
| `←` / `→` | 좌우 한 칸 이동 |
| `↓` | 아래로 한 칸 이동 |
| `↑` | 시계 방향 90도 회전. O 조각은 회전하지 않음 |
| `Space` | 내려갈 수 있는 마지막 위치까지 즉시 낙하하고 고정 |
| `Esc` | 종료 |
| GameOver에서 `R` | 새 보드의 게임 Scene으로 다시 진입 |

키 입력은 `GetKeyDown()`으로 처리하므로 누르기 시작한 프레임에 한 번 적용됩니다. 자동 낙하는 입력과 별도로 경과 시간을 누적해 처리합니다.

공유 솔루션 [HiwoongEngine.sln](../HiwoongEngine/HiwoongEngine.sln)의 `AsciiTetris` 프로젝트로 실행합니다. 공통 빌드·실행 환경은 [루트 README](../README.md)를 참고하세요. 진입점은 [Main.cpp](Main.cpp)이며, 콘솔 출력 객체를 엔진에 전달하고 `TestScene`을 등록합니다.

## 책임을 나눈 구조

| 구성 요소 | 담당하는 책임 | 구현 |
|---|---|---|
| `TestScene` | 맵·보드·UI 생성, 레벨업과 GameOver 전환 | [TestScene.cpp](Scene/TestScene.cpp) |
| `TetrisModule` | 네 칸으로 구성한 조각의 이동·회전·낙하·고정 | [TetrisModule.cpp](Interface/TetrisModule.cpp) |
| `PlayerInputComponent` | 키를 조각의 동작 요청으로 변환 | [PlayerInputComponent.cpp](GameObject/Player/PlayerInputComponent.cpp) |
| `Block` | `SpriteRendererComponent`로 색상이 있는 `@` 한 칸 표시 | [Block.cpp](GameObject/TetrisBlocks/Block.cpp) |
| `TetrisBoard` | 셀 점유 검사, 완성된 줄 제거와 위쪽 블록 이동 | [TetrisBoard.cpp](GameObject/BackGround/TetrisBoard.cpp) |
| `SpawnManager` | 조각 생성, 다음 조각 예약, 생성 위치의 점유 검사 | [SpawnManager.cpp](GameObject/SpawnManager/SpawnManager.cpp) |
| `TetrisGameState` | Scene을 넘어 유지할 점수·레벨·목표 점수·다음 조각 번호 | [TetrisGameState.cpp](Manager/TetrisGameState.cpp) |
| `GameStatesUI` | 공유 상태를 읽어 점수·레벨·다음 조각 표시 | [GameStatesUI.cpp](Scene/GameStatesUI.cpp) |

`TetrisModule`과 `Block`은 `GameObject`이고, 입력과 문자 출력은 각각 Component로 붙입니다. 보드의 점유 데이터는 `TestScene`이 소유하는 별도 객체에 두어, 화면에 그려진 문자와 게임 규칙을 분리했습니다.

## 조각 생성부터 줄 제거까지

<img width="1400" height="560" alt="조각 생성, 이동과 회전 검사, 고정, 줄 제거, 상태 갱신의 흐름" src="https://github.com/user-attachments/assets/e161f7e8-0750-45d8-aefa-187838eec2c2" />

1. `SpawnManager`가 예약된 번호로 I·O·T·L·J·S·Z 중 하나를 생성합니다.
2. 새 조각의 네 칸이 생성 위치에서 비어 있는지 확인합니다. 막혀 있으면 GameOver를 요청합니다.
3. `TetrisModule`이 자식 `Block` 네 개와 입력 Component를 준비하고, 다음 조각 번호를 UI에서 확인할 수 있습니다.
4. 이동·회전 요청마다 후보 좌표를 보드에 조회하고, 가능한 경우에만 좌표를 바꿉니다.
5. 자동 낙하가 막히거나 즉시 낙하가 끝나면 네 Block을 보드 셀에 등록하고 완성된 줄을 제거합니다.
6. 고정 완료 콜백으로 `SpawnManager`에 알리면 다음 조각을 생성합니다.

### 네 개의 Block을 하나의 조각으로 움직이기

각 조각 클래스는 네 개의 로컬 좌표, 색상, 회전 중심을 설정하고 공통 동작을 `TetrisModule`에서 재사용합니다. 조각별 선언은 [TetrisBlocks/TetrisModule.h](GameObject/TetrisBlocks/TetrisModule.h)에 모여 있습니다.

`TetrisModule::Start()`는 Scene에 Block 네 개를 생성하고 `SetParent(..., false)`로 자신을 부모로 연결합니다. 부모 조각의 위치를 바꾸면 자식 Block의 월드 위치도 함께 바뀝니다.

```text
Block 월드 위치 = TetrisModule 월드 위치 + Block 로컬 위치
```

실제 객체의 수명은 Scene이 관리하며, 조각이 보관하는 Block 참조와 보드가 보관하는 Block 참조는 `weak_ptr`입니다. 부모·자식 관계와 좌표 계산은 엔진의 [GameObject](../HiwoongEngine/src/GameObject/GameObject.cpp)와 [TransformComponent](../HiwoongEngine/src/Component/TransformComponent.cpp)를 사용합니다.

### 이동과 회전을 적용하기 전에 점유 검사

`CanMove()`는 조각의 월드 위치, 각 Block의 로컬 위치, 이동 방향을 더해 후보 셀 네 개를 계산합니다. `TetrisBoard::IsOccupied()`가 모두 비어 있다고 판단할 때만 부모 조각을 이동합니다. 보드 바깥도 점유된 것으로 처리하므로 같은 검사로 경계와 쌓인 블록을 막습니다.

회전은 회전 중심에 대한 상대 좌표 `(x, y)`를 `(-y, x)`로 바꾸는 방식입니다. 화면의 아래쪽이 양의 y축이므로 시계 방향 90도 회전이 됩니다. 네 후보 셀을 모두 검사한 뒤 로컬 좌표를 일괄 적용해, 충돌한 회전은 원래 모양을 유지합니다.

O 조각은 회전을 비활성화하며, 나머지는 조각별 정수 좌표 중심을 사용합니다. 벽에 닿았을 때 위치를 보정하는 회전 기능은 구현하지 않았습니다.

### 보드 데이터와 표시 객체를 함께 갱신

`TetrisBoard`는 `y * width + x`로 셀을 찾는 1차원 배열 두 개를 사용합니다. `cells`는 벽과 고정된 Block의 점유 여부를, `blockCells`는 해당 셀의 Block에 대한 약한 참조를 보관합니다. 이동 중인 조각은 고정되기 전까지 보드에 등록하지 않습니다.

현재 [map.txt](../HiwoongEngine/Assets/Stages/map.txt)는 테두리를 포함해 12×22칸이며, 플레이 영역은 10×20칸입니다. `TestScene::LoadMap()`이 `#` 위치에 벽을 생성하고 같은 좌표를 보드에 등록합니다.

조각을 고정하면 Block의 월드 좌표를 보드에 기록하고, 테두리를 제외한 각 줄이 가득 찼는지 검사합니다. 완성된 줄의 Block은 삭제를 예약하고 셀을 비웁니다. 그 위쪽을 아래에서 위로 순회하며 Block의 위치와 두 배열을 한 칸씩 내려, 이미 이동한 Block을 다시 처리하지 않습니다.

## 점수·레벨과 Scene 전환

`TetrisGameState`는 함수 내부의 정적 인스턴스로 유지됩니다. Scene을 교체해 보드를 새로 구성해도 점수와 레벨, 다음 조각 번호를 이어 사용할 수 있습니다.

| 규칙 | 현재 구현 |
|---|---|
| 줄 제거 점수 | 한 줄당 10점 |
| 시작 상태 | 0점, 레벨 1, 다음 레벨 목표 100점 |
| 레벨업 | 누적 점수가 목표 이상이면 레벨 +1, 다음 목표 +200점 |
| 자동 낙하 간격 | `max(0.1, 1.0 - (레벨 - 1) * 0.1)`초 |
| 레벨 전환 | 새 `TestScene`으로 교체해 보드를 초기화하고 공유 상태 유지 |
| GameOver | 다음 조각의 생성 위치가 점유되어 있으면 `GameOver` Scene으로 교체 |

레벨 1은 1초마다, 레벨 2는 0.9초마다 한 칸씩 자동 낙하하며 최소 간격은 0.1초입니다. 레벨업은 `TestScene::Update()`에서 판단하고, 새 조각이 시작될 때 해당 레벨의 낙하 간격을 가져옵니다.

GameOver에서 `R`을 누르면 새 `TestScene`으로 들어갑니다. 현재 [GameOver.cpp](Scene/GameOver.cpp)는 `TetrisGameState::Reset()`을 호출하지 않으므로 **점수·레벨·다음 조각을 유지한 채 보드만 새로 시작**합니다. 상태 초기화 함수 자체는 [TetrisGameState](Manager/TetrisGameState.cpp)에 정의되어 있습니다.

## 다음 조각과 상태 UI

`SpawnManager`는 예약된 번호로 현재 조각을 만든 뒤, 0~6 범위에서 다음 번호를 뽑아 공유 상태에 저장합니다. 매번 범위 내 난수를 뽑는 방식이며, 7종을 한 번씩 섞어 소진하는 방식은 사용하지 않습니다.

`GameStatesUI`는 점수·레벨·목표 점수·다음 번호가 이전 표시 값과 달라졌을 때 텍스트를 갱신합니다. 미리보기는 `@`를 표시하는 `TextObject` 네 개의 위치와 색상을 조각 종류에 맞게 변경합니다. 게임 보드용 Block을 생성하지 않고, 엔진의 `SpriteRendererComponent`를 사용한 [TextObject](GameObject/TextObject.h)를 재사용합니다.

## 구현에서 확인할 수 있는 점

- **엔진 기능의 게임 적용:** 입력 Component와 문자 렌더러를 조합하고, 부모 Transform으로 네 Block을 함께 이동합니다.
- **화면과 규칙의 일치:** 줄을 지울 때 점유 배열, Block 참조, 실제 객체 위치를 함께 갱신합니다.
- **생성과 동작의 분리:** 조각은 고정 완료를 콜백으로 알리고, 다음 조각 생성은 `SpawnManager`가 담당합니다.
- **Scene과 상태의 수명 분리:** 보드를 포함한 Scene을 교체하면서 공유 점수·레벨을 UI와 게임 규칙에서 읽습니다.

엔진의 공통 구조와 다른 적용 사례는 [HiwoongEngine README](../README.md)에서 확인할 수 있습니다.
