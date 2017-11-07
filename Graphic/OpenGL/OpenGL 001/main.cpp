#include "MyOpenGLWindow.h"

class MyWindow : public MyOpenGLWindow {
public:
	virtual void onDestroy() override {
		PostQuitMessage(0);
	}

	virtual void onPaint() override {
		glClearColor(0.2f, 0.5f, 0.6f, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
			glColor4f(1,0,0,1); glVertex2f( 0,     0.6f);
			glColor4f(0,1,0,1); glVertex2f( 0.5f, -0.4f);
			glColor4f(0,0,1,1); glVertex2f(-0.5f, -0.4f);
		glEnd();

		swapBuffers();
	}
};

int main() {
	MyWindow w;
	w.create();

    MSG msg;  
    while (GetMessage(&msg, NULL, 0, 0)) {  
        TranslateMessage(&msg);  
        DispatchMessage(&msg);  
    }

	return msg.wParam;

	return 0;
}