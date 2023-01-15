


xcopy	/y/s	.\Engine\Public\*.*			.\Reference\Headers\
xcopy	/y	.\Engine\Bin\*.lib				.\Reference\Librarys\
xcopy	/y	.\Engine\ThirdPartyLib\*.lib	.\Reference\Librarys\
xcopy	/y	.\Engine\Bin\*.dll				.\Client\Bin\

xcopy	/y/s	.\Engine\Public\*.*			..\ImGui\Reference\Headers\
xcopy	/y	.\Engine\Bin\*.lib				..\ImGui\\Reference\Librarys\
xcopy	/y	.\Engine\ThirdPartyLib\*.lib	..\ImGui\Reference\Librarys\
xcopy	/y	.\Engine\Bin\*.dll				..\ImGui\Client\Bin\