#pragma once

#include "IPlug_include_in_plug_hdr.h"

class PresetMenu : public IPanelControl
{
private:
	WDL_String mPreviousPath;

public:
	PresetMenu(IPlugBase *pPlug, IRECT pR)
		: IPanelControl(pPlug, pR, &COLOR_BLUE)
	{}

	bool Draw(IGraphics* pGraphics)
	{
		pGraphics->FillIRect(&COLOR_WHITE, &mRECT);

		int ax = mRECT.R - 8;
		int ay = mRECT.T + 4;
		int bx = ax + 4;
		int by = ay;
		int cx = ax + 2;
		int cy = ay + 2;

		pGraphics->FillTriangle(&COLOR_GRAY, ax, ay, bx, by, cx, cy, &mBlend);

		return true;
	}

	void OnMouseDown(int x, int y, IMouseMod* pMod)
	{
		if (pMod->L)
		{
			doPopupMenu();
		}

		Redraw(); // seems to need this
		SetDirty();
	}

	void doPopupMenu()
	{
		IPopupMenu menu;

		IGraphics* gui = mPlug->GetGUI();

		menu.AddItem("Previous preset");
		menu.AddItem("Next preset");
		menu.AddSeparator();
		menu.AddItem("Save Program...");
		menu.AddItem("Save Bank...");
		menu.AddSeparator();
		menu.AddItem("Load Program...");
		menu.AddItem("Load Bank...");
		menu.AddSeparator();
		menu.AddItem("Dump preset...");

		if (gui->CreateIPopupMenu(&menu, &mRECT))
		{
			const char* paramEnumNames[] =
			{
				"delayTime",
				"lfoAmount",
				"lfoFrequency",
				"driftAmount",
				"tempoSyncTime",
				"feedback",
				"stereoOffset",
				"panMode",
				"pan",
				"duckAmount",
				"duckAttackSpeed",
				"duckReleaseSpeed",
				"lpMode",
				"lpCut",
				"hpCut",
				"driveGain",
				"driveMix",
				"driveFilter",
				"dryVolume",
				"wetVolume",
			};

			int itemChosen = menu.GetChosenItemIdx();
			WDL_String fileName;

			//printf("chosen %i /n", itemChosen);
			switch (itemChosen)
			{
			case 0:
				mPlug->RestorePreset(mPlug->GetCurrentPresetIdx() - 1);
				break;
			case 1:
				mPlug->RestorePreset(mPlug->GetCurrentPresetIdx() + 1);
				break;
			case 3: //Save Program
				fileName.Set(mPlug->GetPresetName(mPlug->GetCurrentPresetIdx()));
				GetGUI()->PromptForFile(&fileName, kFileSave, &mPreviousPath, "fxp");
				mPlug->SaveProgramAsFXP(&fileName);
				break;
			case 4: //Save Bank
				fileName.Set("IPlugChunksBank");
				GetGUI()->PromptForFile(&fileName, kFileSave, &mPreviousPath, "fxb");
				mPlug->SaveBankAsFXB(&fileName);
				break;
			case 6: //Load Preset
				GetGUI()->PromptForFile(&fileName, kFileOpen, &mPreviousPath, "fxp");
				mPlug->LoadProgramFromFXP(&fileName);
				break;
			case 7: // Load Bank
				GetGUI()->PromptForFile(&fileName, kFileOpen, &mPreviousPath, "fxb");
				mPlug->LoadBankFromFXB(&fileName);
				break;
			case 9:
				fileName.Set("preset");
				GetGUI()->PromptForFile(&fileName, kFileSave, &mPreviousPath, "txt");
				mPlug->DumpPresetSrcCode(fileName.Get(), paramEnumNames);
				break;
			default:
				break;
			}
		}
	}
};