<div align="center">

# [DirectX11] Tales of Arise 모작

<img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/029ab07d-351e-4622-bc9e-e0e17260fc17" width="50%"/>

개발기간 : 2개월
  
개발인원 : 5명
  
##### 📌해당 설명서는 팀원 개개인이 개별적으로 작성하기에 팀원 모두의 구현 내용을 담은 것이 아닙니다. 
#####  추후 팀원들의 개별 작성을 통해 main branch 에는 팀원들의 전체 내용이, 
##### 각 팀원별 branch에는 개인이 담당한 파트 일부분이 기록될 예정입니다. 이 점 참고바랍니다.
  
</div>

<br></br>


# *동영상/GitHub*

[130기 테일즈오브어라이즈 포트폴리오 영상](https://youtu.be/cACuolSqf2g)

# *담당파트*


- ***혜연 : 조장, 프레임워크, 카메라, 맵툴***
- ***지훈 : 애니메이션, 플레이어***
- ***정원 : UI, AI***
- ***조이 : 이펙트, 쉐이더***
- ***석희 : 몬스터, 사운드***

# *게임 주요 사진*


### 스테이지 구성

스테이지는 크게 메인필드, 배틀 존, 미니게임, 보스룸으로 구성되어있습니다.

<img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/a97e0d9d-fe7f-4026-81cc-8892c9db29b9" width="40%"/> <img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/521a4eeb-862d-4fac-bdc4-6df8f60fad98" width="40%"/>

<img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/b8d8d1a0-bb2c-443f-9cf3-5c174c2e8f84" width="40%"/> <img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/bbd0c6d4-8d04-4be6-9411-772002cb987e" width="40%"/>

<img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/a194cb49-c842-4862-81c3-68732d47a9f1" width="40%"/> <img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/83f91746-ebf0-49c9-bb4d-1ab7596c6f48" width="40%"/>

# *구현 컨텐츠 및 기능 설명*


## 1. 프레임워크

### 스레드/오브젝트 풀링

![로딩 스레드](https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/a1ea6f29-28ab-4e6e-ba13-3ee0f415e8df)

- 게임에서 신이 전환되면서 로딩 없이 게임이 바로 진행될 수 있도록 처음 게임을 실행할 때 각 레벨마다 필요한 객체들을 프로토타입 패턴을 이용해 클론형으로 생성했습니다. 이는 스레드를 다중으로 이용하여 시간이 지연되는 것을 방지했습니다.
- 이렇게 클론된 객체들을 각 필요한 용도에 따라 Layer별로 분류했고, 오브젝트 풀링을 이용해 풀링 레이어 공간에 담아두었다가 필요할 때 바로 꺼내쓸 수 있도록 구현했습니다.
- 덕분에 객체 생성, 레벨 전환에 있어서 빠른 속도를 유지할 수 있었습니다.

### 플레이 존/배틀존

<img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/4e7e6655-dbdf-4339-9c13-90ce3dadb3dc" width="40%"/> <img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/d99376a8-fbb7-41ef-a0cf-4978a0971b97" width="40%"/>

![ObjectPool](https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/25226d9c-193c-4a72-aacb-61db1dd8f068)

- 해당 게임은 전투와 비전투 상태가 분류되어서 진행되는 게임입니다.
- 이를 구현하기 위해 싱글톤 형태로 배틀에 대한 정보를 관리하는 Battle Manager를 구현했습니다.
- 특정 몬스터와 충돌하면 전투 대상이 해당 몬스터로 설정되고, 해당 몬스터와 플레이어만 배틀존으로 이동하여 배틀존에서 전투를 진행하는 형태입니다.
- 몬스터가 죽고 배틀이 종료되면 다시 일반 필드로 씬이 전환되고 오브젝트 풀링을 이용해 필드에 있던 데이터들을 로드했습니다.

### 메인 플레이어 / AI 플레이어 제어

- 테일즈오브 어라이즈의 특징은 컨트롤할 메인 플레이어가 전환 가능하다는 점입니다.
- 전투/비전투 상황에 플레이어가 자유롭게 전환이 가능하고 이에 따라 기존에 플레이어였던 대상이 AI로 전환되게 처리했습니다.
- 이를 구현하기 위해 플레이어와 AI의 정보를 담고 AI와 플레이어의 상태를 확인하는 PlayerManager를 싱글톤 패턴으로 구현했습니다.

### 상태 패턴

![상태 패턴](https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/6f1ac1b6-03ef-442d-9aea-9e90c39371fb)

![FlowchartDiagram1](https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/48b10a25-f2df-4395-a6d6-66d7c3fa46ea)

- 팀프로젝트를 하면서 업무 분할과 수정, 가독성 등에 있어서 효율적으로 진행하고자 상태 패턴을 활용했습니다.
- 객체들의 움직임들을 각 상태에 따라 분할하여 제어하도록 처리했습니다.

## 2. 배틀 컨텐츠

### 피격 / 타격 제어

![제목 없는 동영상 - Clipchamp로 제작 (4)](https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/82ffa816-5a00-4d64-bcbb-6f6f41ef0d06)

- 타격감이나 피격, 타격에 따른 처리를 위한 상세한 디테일을 처리했습니다.
- 추상 클래스에서 가진 Take_Damage 함수를 상속받아 세부적으로 구현했고, 피격을 받을 시 데미지의 타입이나, 쉐이킹, 크리티컬 등 다양한 피격 처리를 진행했습니다.

### LOCKON / OFF 기능

![제목 없는 동영상 - Clipchamp로 제작 (3)](https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/539f9ed2-267f-48ec-91e3-8dc809b32d83)

- 공격시 타겟몬스터를 설정할 수 있는 LockOn 기능이 있습니다.
- 각 몬스터마다 LockOn 게이지를 보유하고 있으며, 플레이어는 타겟팅한 대상의 LockOn 게이지를 확인할 수 있습니다.
- 추가적으로 CapsLock 키를 통해 시간을 정지 시켜 타겟 몬스터의 특정 상태를 확인할 수 있습니다.
- 정지 상태일 때 Z와 X를 통해 타겟을 전환할 수 있습니다. 이 때 카메라가 타겟을 따라가며 전환됩니다.
- 해당 타겟은 플레이어와 AI들이 공격하는 우선순위 대상이며 타겟이 죽으면 다른 몬스터가 Lock On 마크가 생기며 타겟팅 됩니다.

### 플레이어 전환

![제목 없는 동영상 - Clipchamp로 제작 (2)](https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/6f0a04f0-5138-4fdd-8f17-1a3a6921713c)

- 위의 일시정지 기능을 통해 동일하게 플레이어도 변경가능합니다.
- 1,2,3,4를 통해 원하는 캐릭터를 메인 플레이어로 설정할 수 있습니다.

### 부스트 어택

![제목 없는 동영상 - Clipchamp로 제작 (5)](https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/8d78dd25-8766-4c55-8fcc-9c87692daec2)

- 플레이어와 AI의 게이지가 다 채워질 경우 부스트 어택을 사용할 수 있습니다. 이때 부스트어택 사용시 카메라가 부스트 어택을 사용한 플레이어로 변환되었다가 다시 돌아옵니다.

### 스트라이크 어택

<img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/cf78eddd-d4d7-46d4-91a1-9843c1b0b39d" width="40%"/> <img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/e99f0b4c-44a8-4e77-aa9f-fa21052ec8b0" width="40%"/>

<img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/8e0a7c3c-cbb4-44af-9475-ec37785d4e0f" width="40%"/> <img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/f66137ac-19d0-4d42-9f2c-f90e71b75cca" width="40%"/>


- 몬스터의 LockOn 게이지가 다 채워지면 시간이 멈추면서 스트라이크 어택(궁극기)를 사용할 수 있게 됩니다.
- 선택하는 플레이어에 따라 카메라 툴로 제작한 액션카메라로 카메라가 전환됩니다.
- 이때 공격하고자 하는 타이밍과 대상의 위치를 조절하여 이펙트 생성, 공격이 가능하고 액션이 끝날 시 9999데미지가 입혀집니다.

## 2. 카메라

### 일반 Camera

<img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/e4e05d0f-6055-469e-b319-5e40d453590a" width="40%"/> <img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/65106f7a-3294-49c1-82a4-c0762dc47112" width="40%"/>

마우스의 움직임을 입력받아서 x의 이동량과 y의 이동량에 따라 플레이어를 공전하는 카메라를 구현했습니다.

### 배틀 Camera

<img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/7716fd6a-49ae-4c4a-b527-ec2306dc12c4" width="40%"/> <img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/e787920d-7ded-4a7f-ae78-e3255472dfff" width="40%"/>

플레이어와 타겟 몬스터를 하나의 화면 안에 담기위해 카메라용 가상 절두체를 생성하여 타겟인 몬스터가 절두체를 벗어나려고 할 때 카메라를 자동으로 회전시키게 했습니다.

### Target Camera

카메라 툴을 이용하여 대상을 바라보며 움직임을 처리하는 타겟 카메라를 구현했습니다.

### Action Camera

NPC와의 대화나, 스트라이크 스킬을 비롯한 액션씬을 카메라 툴로 구현했습니다.

### 기타 이벤트 카메라

미니게임 진행을 위한 미니게임 카메라가 있습니다.

## 3. Tool 기능

### Terrain Tool

<img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/ebd2d865-7251-4eea-bdfa-16a0f51f2b22" width="40%"/> <img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/a87d0439-696f-42b5-9292-63a1ad137584" width="40%"/>

- 필터 텍스처를 이용해 텍스처 스플래팅 기법을 구현했습니다. 지형에 마우스 픽킹을 통해 동적으로 필터 텍스처를 생성 가능하였고, 그것을 bmp파일로 저장 하고 불러오기가 가능하게 했습니다. 이를 레벨마다 다른 텍스처들을 융합하여 스플래팅으로 섞이도록 처리했습니다.
- Height Map으로 지형을 불러오기가 가능하게 하였고, 마우스 픽킹을 통해 모양을 세부적으로 조절할 수 있게 처리했습니다.
- 브러쉬 Shader로 마우스의 픽킹 위치와 범위를 실시간으로 볼 수 있게 했습니다.

### Model Tool

<img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/ba337953-fa1d-4343-801e-d8472c5ca450" width="40%"/> <img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/77927c23-db50-487a-b7d4-e422e6a74ea5" width="40%"/>

- IMGUI툴을 이용하여 객체들을 설치하고 저장, 불러오기 기능을 하는 맵툴을 구현했습니다.
- 각 객체들을 Layer별로 설치하고 삭제할 수 있으며 해당 Layer별로 저장이 가능하게 했습니다. 저장은 가독성과 편의성을 위해 다이얼로그 기능을 구현했습니다.
- 또한 객체의 사이즈, 회전축, 등의 행렬정보를 저장하게 했습니다.
- 동일한 객체의 경우 모델 인스턴싱 기법을 통해 한번에 렌더되게 처리했습니다.

### Navigation Tool

- 마우스 픽킹으로 지형의 픽킹 좌표를 구해 네비게이션을 생성하는 기능을 구현했습니다.
- 슬라이딩 벡터를 계산하야 플레이어가 네비게이션 밖을 벗어나도 미끄러지며 이동가능 하도록 처리했습니다.

### Camera Tool

<img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/8969e8cd-e629-4ca6-ab79-b13d778bba81" width="40%"/> <img src="https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/9f374149-3385-458e-b27f-0d9c1e6e3b9b" width="40%"/>

- 카메라가 현재 바라보는 화면을 캡쳐하여 캡쳐한 순간의 카메라의 좌표와, 바라보는 시야의 위치를 저장하게 처리했습니다.
- 해당 캡쳐 내용을 vector 컨테이너에 담아 play 버튼을 누를 시 처음부터 끝까지 캡쳐한 순서대로 카메라의 위치와 시야가 바뀌도록 처리했습니다.
- 이동하는 방식은 Lerp함수 또는 CatRom 함수를 통해 가능하며 이는 툴 상에서 선택 가능하게 처리했습니다.
- 툴로 제작 가능한 카메라는 Target 카메라와 Action 카메라 두 개로 구분되며 용도에 따라 다른 형태로 저장됩니다.

### Light Tool

![제목 없는 동영상 - Clipchamp로 제작 (19)](https://github.com/WANTWON/CopyGame_DirectX11_Tales_of_Arise/assets/106663427/e6aa495e-4394-4fe9-8c79-b7fc3b9f7cf4)

- 레벨별로 Directional Light와 Point Light를 설치 가능하게 했습니다.
- 각 Light 마다 Diffuse , Ambient 등 빛의 속성들을 제어할 수 있고 이를 데이터 파일로 저장 불러오기가 가능하게 처리했습니다.

## *기술적 요소*


자세한 코드 설명은 깃허브 프로젝트 혹은 기술소개서에서 확인 가능합니다.

- 레벨 제어
- 상태 패턴
- 로딩 쓰레드
- 오브젝트 풀링
- 액션 카메라  / 카메라 툴
- 지형 스플래팅, Height Map, 지형 조절 툴
- 타격감 처리
