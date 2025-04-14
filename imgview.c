#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <unistd.h> 
#include <windows.h>

typedef enum { _0, _90, _180, _270 } angler;
void zoom(HWND hwnd, float zoomFactor, int *clientWidth, int *clientHeight, float clientRatio);
void reset(HWND hwnd, int imgWidth, int imgHeight, angler angle, int *clientWidth, int *clientHeight, float *clientRatio);

// 全局窗口过程
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	const int MIN_WIDTH = 20;
	const int MIN_HEIGHT = 20;
	static HBITMAP hBitmap;
	static int clientWidth = 0, clientHeight = 0;
	//static int lastClientWidth = 0, lastClientHeight;
	//保存窗口前一个状态
	static RECT lastRect;
	//static LONG lastStyle;
	static int imgWidth = 0, imgHeight = 0;
	static float clientRatio = 1.0f;
	static POINT lastMousePos = {0}; // 记录鼠标按下时的位置
	static int isFillDesktop = 0, isFullScreen = 0, isFlip = 0, isWindowOnTop = 0;
	static angler angle = _0;
	static uint8_t alpha = 255;
	static HDC hdcMem;

	switch(msg) 
	{
		case WM_CREATE: 
			{
				// 设置窗口为分层窗口
				SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
				SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA);
				// 获取传递的参数
				LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
				hBitmap = (HBITMAP)pcs->lpCreateParams;
				if (!hBitmap) 
				{
					MessageBox(NULL, "LoadImage Failed", "Error", MB_OK | MB_ICONERROR);
					return 0;
				}
				BITMAP bm;
				GetObject(hBitmap, sizeof(bm), &bm);
				imgWidth = bm.bmWidth;
				imgHeight = bm.bmHeight;
				clientRatio = (float)imgWidth / imgHeight;
				clientWidth = imgWidth;
				clientHeight = imgHeight;
				// 创建内存 DC
				hdcMem = CreateCompatibleDC(NULL);
				SelectObject(hdcMem, hBitmap);
				break;
			}
		case WM_KEYDOWN: 
			{
				switch (wParam) 
				{
					//Backspace: reset all
					case 0x08:
						isFullScreen = isFillDesktop = isFlip = isWindowOnTop = 0;
						angle = _0;
						alpha = 255;
					    	SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA);
						clientWidth = imgWidth;
						clientHeight = imgHeight;
						clientRatio = (float)clientWidth / clientHeight;
						InvalidateRect(hwnd, NULL, TRUE);  
						// 获取鼠标位置
						POINT mousePos;
						GetCursorPos(&mousePos);
						//以鼠标位置为中心, 但不超越左边界和右边界
						int x = mousePos.x - imgWidth/2;
						if (x < 0) x = 0;
						int y = mousePos.y - imgHeight/2;
						if (y < 0) y = 0;
						SetWindowPos(hwnd, HWND_NOTOPMOST, x, y, 
								clientWidth, clientHeight, 0);
						break;
					//0
					case 0x30:
						reset(hwnd, imgWidth, imgHeight, angle, &clientWidth, &clientHeight, &clientRatio);
						break;
					//f
					case 0x46:
						{
							int isShift = GetKeyState(VK_SHIFT) & 0x8000;
							int isCaps = GetKeyState(VK_CAPITAL) & 0x0001;
							int isUpper = isShift ^ isCaps; 
							if (isUpper) {
								isFlip = !isFlip;
								InvalidateRect(hwnd, NULL, TRUE);  
								break;
							}
							else if (!isFullScreen) 
							{
								// 保存原始窗口状态
								GetWindowRect(hwnd, &lastRect);
								//lastStyle = GetWindowLong(hwnd, GWL_STYLE);

								// 获取屏幕分辨率
								clientWidth = GetSystemMetrics(SM_CXSCREEN);
								clientHeight = GetSystemMetrics(SM_CYSCREEN);

								SetWindowPos(hwnd, HWND_TOP, 0, 0, clientWidth, clientHeight, SWP_FRAMECHANGED);
								isFullScreen = 1;
							} 
							else 
							{
								clientWidth =lastRect.right - lastRect.left;
								clientHeight = lastRect.bottom - lastRect.top;
								//SetWindowLong(hwnd, GWL_STYLE, lastStyle);
								SetWindowPos(hwnd, NULL, lastRect.left, lastRect.top, 
										clientWidth, 
										clientHeight,
										SWP_NOZORDER);
								isFullScreen = 0;
							}
							break;
						}
					// q	
					case 0x51:
						PostQuitMessage(0);
						break;
					// r	
					case 0x52:
						angle++;
						angle %= 4;
						clientWidth = clientWidth ^ clientHeight;
						clientHeight = clientWidth ^ clientHeight;
						clientWidth = clientWidth ^ clientHeight;
						clientRatio = (float)clientWidth / clientHeight;
						SetWindowPos(hwnd, NULL, 0, 0, 
								clientWidth, clientHeight, SWP_NOZORDER | SWP_NOMOVE);
					//t
					case 0x54:
						isWindowOnTop = !isWindowOnTop;
						SetWindowPos(hwnd, isWindowOnTop ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
						break;
					//+	
					case VK_OEM_PLUS:
						zoom(hwnd, 1.1f, &clientWidth, &clientHeight, clientRatio);
						break;
					//-
					case VK_OEM_MINUS:
						zoom(hwnd, 0.9f, &clientWidth, &clientHeight, clientRatio);
						break;
					case VK_UP:
					case VK_DOWN:
					case VK_LEFT:
					case VK_RIGHT:
						{
							int isCtrlPressed = GetAsyncKeyState(VK_CONTROL) & 0x8000;
							if (isCtrlPressed)
							{
								int offsetY = 0;
								int offsetX = 0;
								switch (wParam)
								{
									case VK_UP: offsetY = -10; break;
									case VK_DOWN: offsetY = 10; break;
									case VK_LEFT: offsetX = -10; break;
									case VK_RIGHT: offsetX = 10; break;
								}
								RECT rect;
								GetWindowRect(hwnd, &rect);
								SetWindowPos(hwnd, NULL, rect.left+offsetX, rect.top+offsetY, 
										0, 0, SWP_NOZORDER | SWP_NOSIZE);
							}
							else 
							{

							}
							break;
						}
					default:
						break;
				}
				break;
			}
		case WM_LBUTTONDBLCLK: 
			{
				if (!isFillDesktop) 
				{
					// 获取桌面工作区（不包括任务栏）
					RECT workArea;
					if (!SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0)) 
					{
						MessageBox(NULL, "Get Desktop area failed", "Error", MB_OK | MB_ICONERROR);
						return 1;
					}
					int desktopWidth = workArea.right - workArea.left;  // 工作区宽度
					int desktopHeight = workArea.bottom - workArea.top; // 工作区高度
					// 保存原始窗口状态
					GetWindowRect(hwnd, &lastRect);
					//lastStyle = GetWindowLong(hwnd, GWL_STYLE);

					if (clientWidth * desktopHeight > clientHeight * desktopWidth)
					{
						clientWidth = desktopWidth;
						clientHeight = (float)clientWidth / clientRatio;
					} 
					else 
					{
						clientHeight = desktopHeight;
						clientWidth = clientRatio * clientHeight;
					}
					SetWindowPos(hwnd, NULL, 0, 0, 
							clientWidth, clientHeight, SWP_NOZORDER);
					isFillDesktop = 1;
				}
				else 
				{
					clientWidth = lastRect.right - lastRect.left;
					clientHeight = lastRect.bottom - lastRect.top;
					//SetWindowLong(hwnd, GWL_STYLE, lastStyle);
					SetWindowPos(hwnd, NULL, lastRect.left, lastRect.top, 
							clientWidth, 
							clientHeight,
							SWP_NOZORDER);
					isFillDesktop = 0;
				}
				break;
			}
		case WM_SIZING: 
			{
				RECT* rect = (RECT*)lParam;
				int newWidth = rect->right - rect->left;
				int newHeight = rect->bottom - rect->top;

				int isCtrlPressed = GetAsyncKeyState(VK_CONTROL) & 0x8000;
				if (!isCtrlPressed) 
				{
					switch (wParam) 
					{
						// 调整左边或右边时，按比例调整高度
						case WMSZ_LEFT:
							newHeight = (int)(newWidth / clientRatio);
							rect->top = rect->bottom - newHeight;
							break;
						case WMSZ_RIGHT:
							newHeight = (int)(newWidth / clientRatio);
							rect->bottom = rect->top + newHeight;
							break;

							// 调整顶部或底部时，按比例调整宽度
						case WMSZ_TOP:
							newWidth = (int)(newHeight * clientRatio);
							rect->left = rect->right - newWidth;
							break;
						case WMSZ_BOTTOM:
							newWidth = (int)(newHeight * clientRatio);
							rect->right = rect->left + newWidth;
							break;

							// 调整角落时（左上、右上、左下、右下），按比例调整
						case WMSZ_TOPLEFT:
						case WMSZ_TOPRIGHT:
							newHeight = (int)(newWidth / clientRatio);
							rect->top = rect->bottom - newHeight;
							break;
						case WMSZ_BOTTOMLEFT:
						case WMSZ_BOTTOMRIGHT: 
							newHeight = (int)(newWidth / clientRatio);
							rect->bottom = rect->top + newHeight;
							break;
					}
				} 
				else 
				{
					clientRatio = (float)newWidth / newHeight;
				}
				clientWidth = newWidth;
				clientHeight = newHeight;
				return TRUE;
			}
		case WM_SIZE:
			isFillDesktop = 0;
			isFullScreen = 0;
			InvalidateRect(hwnd, NULL, TRUE);  
			return 0;
		case WM_MOUSEWHEEL: 
			{
				int isCtrlPressed = GetAsyncKeyState(VK_CONTROL) & 0x8000;
				if (!isCtrlPressed)  // zoom
				{
					int delta = GET_WHEEL_DELTA_WPARAM(wParam);  // 获取滚轮增量
					if (delta < 0 && (clientWidth < MIN_WIDTH || clientHeight < MIN_HEIGHT)) return 0;
					float zoomFactor = (delta > 0)? 1.05f: 0.95f;
					zoom(hwnd, zoomFactor, &clientWidth, &clientHeight, clientRatio);
				}
				else  //set transparency
				{
					int delta = GET_WHEEL_DELTA_WPARAM(wParam);  // 获取滚轮增量
					if (delta < 0)
					{
						if (alpha < 255) alpha += 10; // 增加透明度
					    	SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA);
					}
					else 
					{
						if (alpha > 0) alpha -= 10; // 降低透明度
						SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA);
					}
				}
				return 0;
			}
		case WM_MBUTTONDOWN: //reset img size
			{
				reset(hwnd, imgWidth, imgHeight, angle, &clientWidth, &clientHeight, &clientRatio);
				return 0;
			}
		case WM_PAINT: 
			{
				PAINTSTRUCT ps;
				// 窗口 DC, 每次PAINT都要调用
				HDC hdc = BeginPaint(hwnd, &ps);

				//x′=eM11⋅x + eM12⋅y + eDx
				//y′=eM21⋅x + eM22⋅y + eDy
				XFORM xform = {
				     1.0f, 0.0f, 
				     0.0f, 1.0f, 
				     0.0f, 0.0f
				};
				int drawWidth = clientWidth;
				int drawHeight = clientHeight;

				switch (angle) 
				{
					case _90:
						xform.eM11 = 0;    xform.eM12 = 1;
						xform.eM21 = -1;   xform.eM22 = 0;
						break;

					case _180:
						xform.eM11 = -1;   xform.eM12 = 0;    
						xform.eM21 = 0;    xform.eM22 = -1;   
						break;

					case _270:
						xform.eM11 = 0;    xform.eM12 = -1;   
						xform.eM21 = 1;    xform.eM22 = 0;    
						break;

					default:
					break;
				}

				if (angle == _90 || angle == _270)
				{
					drawWidth = clientHeight;
					drawHeight = clientWidth;
				}

				int x = drawWidth * xform.eM11 -drawHeight * xform.eM12;
				int y = drawWidth * xform.eM21 -drawHeight * xform.eM22;
				{
					if (x < 0)
					{
						xform.eDx = -x;
					}
					if (y > 0)
					{
						xform.eDy = y;
					}
				}

				SetGraphicsMode(hdc, GM_ADVANCED);
				SetWorldTransform(hdc, &xform);
				StretchBlt(hdc, 0, 0, drawWidth, drawHeight,
						hdcMem, 0, 0, imgWidth, imgHeight, SRCCOPY);

				ReleaseDC(NULL, hdc);
				EndPaint(hwnd, &ps);
				break;
			}
		case WM_NCHITTEST: 
			{
				// 获取鼠标位置
				POINT pt = { LOWORD(lParam), HIWORD(lParam) };
				RECT rc;
				GetWindowRect(hwnd, &rc);

				// 定义调整边界的大小
				const int borderWidth = 8;

				// 判断鼠标是否位于窗口边缘
				int left   = pt.x < rc.left + borderWidth;
				int right  = pt.x > rc.right - borderWidth;
				int top    = pt.y < rc.top + borderWidth;
				int bottom = pt.y > rc.bottom - borderWidth;

				// 处理不同边缘的调整区域
				if (left && top) return HTTOPLEFT;
				if (right && top) return HTTOPRIGHT;
				if (left && bottom) return HTBOTTOMLEFT;
				if (right && bottom) return HTBOTTOMRIGHT;
				if (left) return HTLEFT;
				if (right) return HTRIGHT;
				if (top) return HTTOP;
				if (bottom) return HTBOTTOM;

				return HTCLIENT; // 让窗口客户区正常接收事件
			}
		case WM_LBUTTONDOWN: // 左键按住拖动窗口
			ReleaseCapture(); // 释放鼠标控制
			SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0); // 让窗口像标题栏一样拖动
			if (lastMousePos.y < 0)
				SetWindowPos(hwnd, NULL, lastMousePos.x, lastMousePos.y, 0, 0, 
						SWP_NOSIZE | SWP_NOZORDER);
			lastMousePos.x = 0 , lastMousePos.y = 0;
			return 0;
		case WM_MOVE: 
			{
				RECT rect;
				GetWindowRect(hwnd, &rect);
				if (!(rect.top == 0 && lastMousePos.y < 0))
					lastMousePos.x = rect.left, lastMousePos.y = rect.top;
				return 0;
			}

		case WM_RBUTTONUP:
			PostQuitMessage(0);
			return 0;
		case WM_GETMINMAXINFO: 
			{
				// 处理最小尺寸限制
				MINMAXINFO* pInfo = (MINMAXINFO*)lParam;
				pInfo->ptMinTrackSize.x = MIN_WIDTH; // 最小宽度
				pInfo->ptMinTrackSize.y = MIN_HEIGHT; // 最小高度
				return 0;
			}
		case WM_ERASEBKGND:
			return 1;  

		case WM_DESTROY:
			if(hBitmap)
				DeleteObject(hBitmap);
			if(hdcMem)
				DeleteDC(hdcMem);
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, 
		HINSTANCE hPrevInstance, 
		LPSTR lpCmdLine, 
		int nCmdShow) 
{
	printf("%s \n", lpCmdLine);
	//printf("%s \n", imagePath);
	if (lpCmdLine[0] == '"') 
	{
		lpCmdLine++;
		for (int i = 0; lpCmdLine[i]; i++)
		{
			if (lpCmdLine[i] == '"')
				lpCmdLine[i] = '\0';
		}
	}
	printf("img path: %s \n", lpCmdLine);

	char errmsg[256] = {0};
	// 加载图片
	int width, height, channels;
	unsigned char* img = stbi_load(lpCmdLine, &width, &height, &channels, 4);
	if (!img) 
	{
		sprintf(errmsg, "stbi_load failed: file path: \"%s\"", lpCmdLine);
		MessageBox(NULL, errmsg, "Error", MB_OK | MB_ICONERROR);
		return -1;
	}
	// 分配内存（纯 C 使用 malloc）
	unsigned char* bgraData = (unsigned char*)malloc(width * height * 4);
	if (!bgraData) return -2; // 检查内存分配是否成功

	// 转换 RGBA → BGRA
	for (int i = 0; i < width * height; ++i) {
		bgraData[i * 4 + 0] = img[i * 4 + 2]; // B
		bgraData[i * 4 + 1] = img[i * 4 + 1]; // G
		bgraData[i * 4 + 2] = img[i * 4 + 0]; // R
		bgraData[i * 4 + 3] = img[i * 4 + 3]; // A
	}

	// 创建 BITMAPINFO 结构
	BITMAPINFO bmi = {0};
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height; // 负数表示从上到下
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32; // 每个像素的位数
	bmi.bmiHeader.biCompression = BI_RGB;

	// 创建位图
	HDC hdc = GetDC(NULL);
	HBITMAP hBitmap = CreateDIBitmap(hdc, &bmi.bmiHeader, CBM_INIT, bgraData, &bmi, DIB_RGB_COLORS);
	ReleaseDC(NULL, hdc);

	free(bgraData);
	stbi_image_free(img);

	//    // 加载BMP文件
	//    HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, 
	//			       imagePath, // 替换为你的BMP文件路径
	//			       IMAGE_BITMAP,
	//			       0,
	//			       0,
	//			       LR_LOADFROMFILE);
	if(!hBitmap) 
	{
		MessageBox(NULL, "LoadImage Failed", "Error", MB_OK | MB_ICONERROR);
		return -2;
	}
	BITMAP bm;
	GetObject(hBitmap, sizeof(bm), &bm);
	int imgWidth = bm.bmWidth;
	int imgHeight = bm.bmHeight;

	// 注册窗口类
	WNDCLASS wc = {0};
	//WM_LBUTTONDBLCLK
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = "IMGViewer";
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);  // 设置为标准箭头光标, 否则鼠标变成loading或者resize之后无法恢复
	RegisterClass(&wc);

	// 获取鼠标位置
	POINT mousePos;
	GetCursorPos(&mousePos);
	//以鼠标位置为中心, 但不超越左边界和右边界
	int x = mousePos.x - imgWidth/2;
	if (x < 0) x = 0;
	int y = mousePos.y - imgHeight/2;
	if (y < 0) y = 0;

	// 创建窗口（初始大小设为0，因为会在WM_CREATE中调整）
	HWND hwnd = CreateWindow(
			"IMGViewer",
			"Image Viewer",
			WS_POPUP,
			x,
			y,
			imgWidth,
			imgHeight,
			NULL,
			NULL,
			hInstance,
			hBitmap);

	if(!hwnd) 
	{
		MessageBox(NULL, "Window creation failed!", "Error", MB_OK | MB_ICONERROR);
		return -3;
	}

	// 显示窗口
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// 消息循环
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}


void zoom(HWND hwnd, float zoomFactor, int *clientWidth, int *clientHeight, float clientRatio) 
{
	POINT pt;
	GetCursorPos(&pt); 
	RECT rect;
	GetWindowRect(hwnd, &rect);
	int newLeft = pt.x - ((pt.x - rect.left) * zoomFactor);
	int newTop = pt.y - ((pt.y - rect.top) * zoomFactor);
	*clientHeight = (rect.bottom - rect.top) * zoomFactor;
	*clientWidth = (float)*clientHeight * clientRatio;

	//todo optimize flicker
	SetWindowPos(hwnd, NULL, newLeft, newTop, 
			*clientWidth, *clientHeight, SWP_NOZORDER);
}

void reset(HWND hwnd, int imgWidth, int imgHeight, angler angle, int *clientWidth, int *clientHeight, float *clientRatio)
{
	int newWidth, newHeight;
	if (angle == _90 || angle == _270)
	{
		newWidth = imgHeight;
		newHeight = imgWidth;
	} 
	else 
	{
		newWidth = imgWidth;
		newHeight = imgHeight;
	}
	POINT ptScreen;
	GetCursorPos(&ptScreen); 
	POINT pt = ptScreen;
	ScreenToClient(hwnd, &pt); 

	RECT rect;
	GetWindowRect(hwnd, &rect);
	int newLeft = pt.x - (float)pt.x / *clientWidth * newWidth +  rect.left;
	int newTop = pt.y - (float)pt.y / *clientHeight * newHeight + rect.top;

	*clientWidth = newWidth;
	*clientHeight = newHeight;

	*clientRatio = (float)*clientWidth / *clientHeight;

	// 设置新窗口大小（保持中心位置不变）
	SetWindowPos(hwnd, NULL, newLeft, newTop, 
			*clientWidth, *clientHeight, SWP_NOZORDER);
}
