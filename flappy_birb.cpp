#include<iostream>
#include<string>

using namespace std;

#include "OLCEngine.h"

class Console_Bird : public olcConsoleGameEngine {
public:
	Console_Bird() {
		m_sAppName = L"Flappy Bird";
	}
private:
	float birdPos = 0.0f;
	float birdVel = 0.0f;
	float birdAccel = 0.0f;
	
	float gravity = 100.0f;

	float beamWidth;
	list<int> listSection;
	float levelPosition = 0.0f;

	bool bHasCollided = false;
	bool bResetGame = true;

	int nAttempts = 0;
	int nFlaps = 0;
	int nMaxFlaps = 0;

protected:
	void drawCenteredText(int y, wstring text) {
		DrawString((int)(ScreenWidth()/2) - (int)(text.length() / 2), y, text);
	}
	virtual bool OnUserCreate() {

		listSection = { 0,0,0,0 };
		beamWidth = (float)ScreenWidth() / (float)(listSection.size() - 1);

		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime) {
		if (bResetGame) {
			bHasCollided = false;
			bResetGame = false;
			listSection = { 0,0,0,0 };
			birdAccel = 0.0f;
			birdVel = 0.0f;
			birdPos = ScreenHeight() / 2.0f;
			nFlaps = 0;
			nAttempts++;
		}
		if (bHasCollided) {
			drawCenteredText(20, L"LMAO DED");
			drawCenteredText(21, L"Score: " + to_wstring(nFlaps));
			if (m_keys[VK_SPACE].bReleased)
				bResetGame = true;
		}
		else {
			if (m_keys[VK_SPACE].bPressed && birdVel >= gravity / 10.0f) {
				birdAccel = 0.0f;
				birdVel = -gravity / 4.0f;
				nFlaps++;
				if (nFlaps > nMaxFlaps)
					nMaxFlaps = nFlaps;
			}
			else
				birdAccel += gravity * fElapsedTime;

			if (birdAccel > gravity)
				birdAccel = gravity;

			birdVel += birdAccel * fElapsedTime;
			birdPos += birdVel * fElapsedTime;

			levelPosition += 14.0f * fElapsedTime;

			if (levelPosition > beamWidth) {
				levelPosition -= beamWidth;
				listSection.pop_front();

				int i = rand() % (ScreenHeight() - 20);
				if (i <= 10) i = 0;
				listSection.push_back(i);

			}

			Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');

			int BirdX = (int)(ScreenWidth() / 3.0f);

			int nSection = 0;
			for (auto s : listSection) {
				if (s != 0) {
					Fill(nSection * beamWidth + 10 - levelPosition, ScreenHeight() - s, nSection * beamWidth + 15 - levelPosition, ScreenHeight(), PIXEL_SOLID, FG_GREEN);

					Fill(nSection * beamWidth + 10 - levelPosition, 0,
						nSection * beamWidth + 15 - levelPosition, ScreenHeight() - s - 15,
						PIXEL_SOLID, FG_GREEN);
				}
				nSection++;

				bHasCollided = birdPos < 2 || birdPos > ScreenHeight() - 2 ||
					m_bufScreen[(int)(birdPos + 0) * ScreenWidth() + BirdX].Char.UnicodeChar != L' ' ||
					m_bufScreen[(int)(birdPos + 1) * ScreenWidth() + BirdX].Char.UnicodeChar != L' ' ||
					m_bufScreen[(int)(birdPos + 0) * ScreenWidth() + BirdX + 6].Char.UnicodeChar != L' ' ||
					m_bufScreen[(int)(birdPos + 1) * ScreenWidth() + BirdX + 6].Char.UnicodeChar != L' ';
			}

			if (birdVel > 0) {
				DrawString(BirdX, birdPos, L"\\\\\\\\");
				DrawString(BirdX, birdPos + 1, L"-\\\\\\\\:D");
			}
			else {
				DrawString(BirdX, birdPos, L"-////:(");
				DrawString(BirdX, birdPos + 1, L"////");
			}

			DrawString(1, 1, L"Score: " + to_wstring(nFlaps) + L"   High Score: " + to_wstring(nMaxFlaps) + L"   Attempts: " + to_wstring(nAttempts) + L"   Get a life man!");
		}

		

		return true;
	}
};

int main() {
	Console_Bird game;
	game.ConstructConsole(80, 48, 16, 16);
	game.Start();

	return 0;
}