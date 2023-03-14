运行环境：
	Keil uVision5（f103，f407芯片支持包）
	Qt5.15.2	< Qt 5.15.2 (MinGW 8.1.0 64-bit) >
	Python 3.10 64-bit

注意事项：

	***机动救援装置：
		1、Qt编译位数应与Python 一致（推荐64-bit），否则 机动救援装置 中脚本无法运行
		2、直接运行上位机要添加Qt环境变量，即Qt中mingw81_64下bin的位置，如（C:\Qt\5.15.2\mingw81_64\bin）
	***智能门禁
		1、直接运行上位机要添加Qt环境变量，即Qt中mingw81_64下bin的位置，如（C:\Qt\5.15.2\mingw81_64\bin）
		2、应使用COM3连接