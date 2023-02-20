#pragma once

namespace Engine
{
	typedef struct tagKeyframe
	{
		float				fTime;
		XMFLOAT3			vScale;
		XMFLOAT4			vRotation;
		XMFLOAT3			vPosition;
	}KEYFRAME;

	typedef struct tagModelMaterial
	{		
		class CTexture*		pMaterials[AI_TEXTURE_TYPE_MAX];
	}MODELMATERIAL;

	/* 빛의 정보를 표현한다. */
	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END };

		TYPE		eType = TYPE_END;
		XMFLOAT4	vPosition;
		float		fRange;
		XMFLOAT4	vDirection;

		XMFLOAT4	vDiffuse;
		XMFLOAT4	vAmbient;
		XMFLOAT4	vSpecular;
	}LIGHTDESC;

	typedef struct tagLineIndices16
	{
		unsigned short		_0, _1;
	}LINEINDICES16;

	typedef struct tagLineIndices32
	{
		unsigned long		_0, _1;
	}LINEINDICES32;

	typedef struct tagFaceIndices16
	{
		unsigned short		_0, _1, _2;
	}FACEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long		_0, _1, _2;
	}FACEINDICES32;

	typedef struct tagVertexPosition
	{
		XMFLOAT3		vPosition;		
	}VTXPOS;

	typedef struct ENGINE_DLL tagVertexPosition_Declaration
	{
		static const unsigned int iNumElements = 1;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXPOS_DECLARATION;

	typedef struct tagVertexTexture
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexture;
	}VTXTEX;

	typedef struct ENGINE_DLL tagVertexTexture_Declaration
	{
		static const unsigned int iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXTEX_DECLARATION;

	typedef struct tagVertexTextureAlpha
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexture;
		float			fAlpha;
		XMFLOAT3		vColor;
	}VTXTEXALPHA;

	typedef struct ENGINE_DLL tagVertexTextureAlpha_Declaration
	{
		static const unsigned int iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXTEXALPHA_DECLARATION;

	typedef struct tagVertexPoint
	{
		XMFLOAT3 vPosition;
		XMFLOAT2 vPSize;
	}VTXPOINT;

	typedef struct ENGINE_DLL tagVertexPoint_Declaration
	{
		static const unsigned int iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXPOINT_DECLARATION;

	typedef struct tagVertexMatrix
	{
		XMFLOAT4 vRight;
		XMFLOAT4 vUp; 
		XMFLOAT4 vLook;
		XMFLOAT4 vPosition;
		float fAlpha;
		XMFLOAT3 vColor;
	}VTXMATRIX;

	typedef struct ENGINE_DLL tagVertexRectInstance_Declaration
	{
		static const unsigned int iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXRECTINSTANCE_DECLARATION;

	typedef struct ENGINE_DLL tagVertexPointInstance_Declaration
	{
		static const unsigned int iNumElements = 8;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXPOINTINSTANCE_DECLARATION;

	typedef struct tagVertexNormalTexture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexture;
		XMFLOAT3		vTangent;
	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertexNormalTexture_Declaration
	{
		static const unsigned int iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXNORTEX_DECLARATION;

	typedef struct tagVertexCubeTexture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexture;
	}VTXCUBETEX;

	typedef struct ENGINE_DLL tagVertexCubeTexture_Declaration
	{
		static const unsigned int iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXCUBETEX_DECLARATION;

	typedef struct tagVertexColor
	{

	}VTXCOL;

	typedef struct tagVertexModel
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT2			vTexUV;
		XMFLOAT3			vTangent;
	}VTXMODEL;

	typedef struct ENGINE_DLL tagVertexModel_Declaration
	{
		static const unsigned int iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXMODEL_DECLARATION;

	/* 애니메이션 재생이 필요한 모델에게 필요한 정점. */
	typedef struct tagVertexAnimModel
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT2			vTexUV;
		XMFLOAT3			vTangent;
		XMUINT4				vBlendIndex;  /* 정점에게 영향을 주는 뼈(최대 네개)의 인덱스*/
		XMFLOAT4			vBlendWeight; /* 정점에게 영향을 주는 뼈상태의 비율 */
	}VTXANIMMODEL;

	typedef struct ENGINE_DLL tagVertexAnimModel_Declaration
	{
		static const unsigned int iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXANIMMODEL_DECLARATION;


	typedef struct ENGINE_DLL tagVertexModelInstance_Declaration
	{
		static const unsigned int iNumElements = 8;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXMODELINSTANCE_DECLARATION;

	typedef struct tagGraphicDesc
	{
		enum WINMODE { MODE_FULL, MODE_WIN, MODE_END };

		unsigned int	iWinSizeX;
		unsigned int	iWinSizeY;
		HWND			hWnd;
		WINMODE			eWinMode;

	}GRAPHIC_DESC;


	// For. Data

	typedef struct tagBinHierarchyNode
	{

		char cName[MAX_PATH];
		char cParent[MAX_PATH];
		int  iDepth;
		XMFLOAT4X4 mTransform;

	}DATA_BINNODE;

	typedef struct tagBinMaterial
	{

		char cNames[AI_TEXTURE_TYPE_MAX][MAX_PATH];

	}DATA_BINMATERIAL;


	typedef struct tagBinBone
	{
		char		cNames[MAX_PATH];
		XMFLOAT4X4	mOffsetTransform;
	}DATA_BINBONE;

	typedef struct tagBinMesh
	{
		char				cName[MAX_PATH];
		int					iMaterialIndex;

		int					NumVertices;
		VTXMODEL*			pNonAnimVertices;
		VTXANIMMODEL*		pAnimVertices;

		int					iNumPrimitives;
		FACEINDICES32*		pIndices;

		int					iNumBones;
		DATA_BINBONE*		pBones;

	}DATA_BINMESH;


	typedef struct tagBinChannel
	{

		char				szName[MAX_PATH];
		int					iNumKeyFrames;
		KEYFRAME*			pKeyFrames;

	}DATA_BINCHANNEL;
	typedef struct tagBinAnim
	{

		bool				bLoop;
		char				szName[MAX_PATH];
		int					iNumChannels;
		float				fDuration;
		float				fTickPerSecond;
		DATA_BINCHANNEL*	pBinChannel;


	}DATA_BINANIM;

	typedef struct tagBinScene
	{

		int iNodeCount;
		DATA_BINNODE* pBinNodes;

		int iMaterialCount;
		DATA_BINMATERIAL* pBinMaterial;

		int iMeshCount;
		DATA_BINMESH* pBinMesh;

		int iNumAnimations;
		DATA_BINANIM* pBinAnim;

	}DATA_BINSCENE;

	typedef struct NonAnimModelTag
	{
		char pModeltag[MAX_PATH] = "";
		XMFLOAT3	vPosition = XMFLOAT3(0.f, 0.f, 0.f);
		XMFLOAT3	vScale = XMFLOAT3(1.f, 1.f, 1.f);
		XMFLOAT3	vRotation = XMFLOAT3(0.f, 0.f, 0.f);
		float		m_fAngle = 0.f;
		XMFLOAT4X4 WorldMatrix;

	}NONANIMDESC;

	typedef struct tagJBinBone
	{
		char szName[MAX_PATH];
		XMFLOAT4X4 Transformation;
		unsigned int iNumChildren;
	}BINBONE;

	typedef struct tagBinAnimation
	{
		char szName[MAX_PATH];
		double dbDuration;
		double dbTickPerSecond;

		int iNumChannels;
	}BINANIM;

	typedef struct tagJBinChannel
	{
		char szName[MAX_PATH];
		int iNumKeyFrames;
	}BINCHANNEL;

	typedef struct tagAnimationEvent
	{
		enum EVENTTYPE { EVENT_SOUND, EVENT_EFFECT, EVENT_COLLIDER, EVENT_STATE, EVENT_INPUT, EVENT_END };

		bool isPlay = false;
		float fStartTime;
		float fEndTime;
		EVENTTYPE eType;
		char szName[MAX_PATH];
	}ANIMEVENT;

	typedef struct tagParticleDesc
	{
		wchar_t wcPrototypeId[MAX_PATH] = TEXT("");
		unsigned int m_eSpawnType = 0;
		bool m_bBillboard = false;
		XMFLOAT3 m_vColor = XMFLOAT3(0.f, 0.f, 0.f);
		float m_fAlpha = 1.f;
		float m_fAlphaDiscard = 0.f;
		bool m_bGlow = false;
		XMFLOAT3 vGlowColor = XMFLOAT3(1.f, 1.f, 1.f);
		int m_iMaxParticles = 1000;
		float m_fParticlesLifetime = 1.f;
		float m_fParticleStartAfter = 0.f;
		float m_fParticlesPerSecond = 1.f;
		float m_fParticleDeviationX = 0.f;
		float m_fParticleDeviationY = 0.f;
		float m_fParticleDeviationZ = 0.f;
		XMFLOAT3 m_vParticleDirection = XMFLOAT3(0.f, 0.f, 0.f);
		bool m_bRandomDirectionX = false;
		bool m_bRandomDirectionY = false;
		bool m_bRandomDirectionZ = false;
		float m_fParticleVelocity = 1.f;
		float m_fParticleVelocityVariation = 0.f;
		float m_fParticleSize = 1.f;
		float m_fParticleSizeVariation = 0.f;
	} PARTICLEDESC;

	struct Particle
	{
		float fPositionX = 0.f, fPositionY = 0.f, fPositionZ = 0.f;
		XMFLOAT3 vDirection = XMFLOAT3(0.f, 0.f, 0.f);
		XMFLOAT3 vInitialColor = XMFLOAT3(0.f, 0.f, 0.f);
		float fRed = 0.f, fGreen = 0.f, fBlue = 0.f;
		float fInitialAlpha = 0.f, fAlpha = 0.f;
		float fInitialVelocity = 0.f, fVelocity = 0.f;
		float fInitialSize = 0.f, fSize = 0.f;
		float fLife = 0.f;
		bool bActive = false;
	};
}
