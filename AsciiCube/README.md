# ASCII Cube
HiwoongEngine의 3D 수학과 소프트웨어 렌더링을 확인하는 ASCII 큐브 데모입니다.
CPU에서 정점을 변환하고 삼각형을 문자 셀로 채워, 회전하는 입체를 Windows 콘솔에 출력합니다.
면의 방향에 따라 문자를 바꾸고 깊이를 비교해 입체감과 가림을 표현합니다.

<br>
</br>

## 플레이 화면

https://github.com/user-attachments/assets/639897e7-3dc8-43dd-8a19-020b15c6207a

Model·View·Projection 변환, 삼각형 채우기, 깊이 버퍼, 법선 기반 ASCII 조명을 적용한 기록입니다.

## 장면과 큐브

[CubeScene](Test/CubeScene.cpp)은 엔진 설정의 화면 크기를 받아 큐브 하나를 생성합니다.
[MeshFactory::CreateCube](../HiwoongEngine/src/Render/MeshFactory.cpp)가 큐브의 기하 데이터를 만듭니다.

| 항목 | 현재 값 |
|---|---|
| 큐브 한 변의 길이 | `2.0` |
| 로컬 정점 범위 | 각 축 `-1`부터 `1`까지 |
| 기하 데이터 | 정점 8개, 모서리 12개, 삼각형 12개 |
| 면 구성 | 사각형 6면을 각각 삼각형 2개로 표현 |
| 초기 위치 | `(0, 0.5, 5)` |
| 초기 회전 | `(0, 0, 0)` |
| 초기 크기 배율 | `(1, 1, 1)` |
| 회전 속도 | X축 `0.5`, Y축 `0.8`, Z축 `0.0` 라디안/초 |

현재 화면은 삼각형 내부를 채워 그립니다. 모서리 데이터는 메시 안에 있지만 와이어프레임 출력에는 사용하지 않습니다.

## 객체와 컴포넌트의 역할

| 구성 요소 | 책임 |
|---|---|
| [CubeObject](Cube/CubeObject.cpp) | 큐브 메시를 보관하고 초기 Transform을 설정하며, `Draw()`에서 3D 그리기를 처리 |
| [CubeRotationComponent](Cube/CubeRotationComponent.cpp) | 매 프레임 회전 속도에 `deltaTime`을 곱해 Transform의 회전을 갱신 |
| [TransformComponent](../HiwoongEngine/src/Component/TransformComponent.cpp) | 위치·회전·크기를 보관하고 Model 행렬을 계산 |
| [SoftwareRasterizer](../HiwoongEngine/src/Render/SoftwareRasterizer.cpp) | 화면 좌표 변환, 뒷면 판정, 삼각형 채우기와 보간 가중치 계산 |
| [Renderer](../HiwoongEngine/src/Render/Renderer.cpp) | 제출된 문자와 깊이를 모아 최종 프레임을 합성하고 출력 |

`GameObject`가 기본 `TransformComponent`를 만들고, `CubeObject::Start()`가 `CubeRotationComponent`를 추가합니다.
회전 컴포넌트는 메시나 콘솔을 다루지 않고 회전값만 바꿉니다. `CubeObject`는 변경된 Model 행렬로 다음 화면을 그립니다.

엔진에는 [MeshRenderer](../HiwoongEngine/src/Render/MeshRenderer.h)라는 공용 3D 그리기 클래스도 있습니다.
이 클래스는 Component를 상속하지 않으며, 메시·Model 행렬·`RenderView`를 받아 그리는 도우미입니다.
현재 Cube는 `MeshRenderer`를 호출하지 않고 `CubeObject::Draw()` 안에서 자체 렌더링 경로를 사용합니다.

## 정점이 문자로 바뀌는 과정

렌더링 흐름은 [CubeObject::Draw()](Cube/CubeObject.cpp)에 들어 있습니다.

1. **Model 변환**: `TransformComponent`에서 큐브의 위치·회전·크기 행렬을 가져옵니다.
2. **View·Projection 변환**: 고정 카메라와 원근 투영 행렬을 곱해 정점을 클립 좌표로 변환합니다.
3. **원근 나누기**: 각 좌표를 `w`로 나누어 정규화 장치 좌표인 NDC로 바꿉니다.
4. **화면 좌표 변환**: NDC를 콘솔 셀 좌표로 옮긴 뒤 Y 좌표에서 15를 빼 화면 위쪽으로 배치합니다.
5. **조명과 뒷면 판정**: 삼각형의 법선을 회전시켜 밝기를 구하고, 화면에서 뒤를 향하는 삼각형은 건너뜁니다.
6. **삼각형 채우기**: 화면 범위 안의 셀을 검사해 삼각형 내부에 들어오는 위치를 구합니다.
7. **깊이 보간과 제출**: 셀의 바리센트릭 가중치로 NDC 깊이를 보간하고 문자·색상·깊이를 `SubmitPoint3D()`에 전달합니다.
8. **프레임 출력**: Renderer가 같은 셀의 깊이를 비교하고, 콘솔 출력기가 두 화면 버퍼를 번갈아 표시합니다.

Model 행렬은 `T × Rz × Ry × Rx × S`, 전체 정점 변환은 `Projection × View × Model` 순서로 구성합니다.
열벡터에 적용할 때는 오른쪽 행렬부터 작용하므로 크기 조절, 회전, 이동 순으로 변환됩니다.

바리센트릭 가중치는 삼각형 안의 한 점을 세 정점의 가중합으로 표현하는 비율입니다.
Cube는 세 정점의 깊이에 이 가중치를 적용합니다. 모든 큐브 셀의 `sortingOrder`는 `0`이며, 같은 순서에서는 더 작은 깊이가 남습니다.

## ASCII 조명

각 삼각형의 두 변을 외적해 로컬 법선을 구하고 회전 행렬로 월드 방향을 계산합니다.
빛 방향은 정규화한 `(-1, -1, -1)`이며, 법선과 빛 방향의 내적을 `0~1`로 제한해 확산광을 구합니다.

```text
밝기 = 0.2 + 0.8 × 확산광
문자 단계 = " .:-=+*#%@"
```

계산한 밝기는 `0~1`로 제한한 뒤 문자 단계의 인덱스로 바꿉니다.
색상은 흰색으로 고정하고 문자의 밀도로 밝기를 표현합니다. 같은 삼각형 안에서는 같은 문자를 사용합니다.

## 카메라와 투영 설정

Cube는 별도의 `Camera3D` 객체 없이 `Draw()`에서 고정 View 행렬을 만듭니다.

| 항목 | 현재 값 |
|---|---|
| 카메라 위치 | `(0, 0, 0)` |
| 바라보는 지점 | `(0, 0, 1)` |
| 위쪽 방향 벡터 | `(0, -1, 0)` |
| 시야각 | `π / 3` 라디안, 약 60도 |
| 근거리·원거리 투영 값 | `0.1`, `100.0` |
| 화면 비율 | 화면 셀의 가로·세로 비율에 실제 문자 크기 비율을 반영 |

문자 크기를 얻지 못하면 [Setting.txt](../HiwoongEngine/Config/Setting.txt)의 `characterAspectRatio`를 사용합니다.
현재 설정 파일의 화면 크기는 `240 × 67`, 목표 프레임률은 `60`, 대체 문자 비율은 `0.5`입니다.

Cube의 자체 경로는 정점의 `w > 0`을 검사하며, 근거리 평면에 걸친 삼각형을 자르는 처리는 하지 않습니다.
엔진의 공용 [MeshRenderer 구현](../HiwoongEngine/src/Render/MeshRenderer.cpp)에는 근거리 평면 클리핑과 스포트라이트 계산이 별도로 구현되어 있습니다.
