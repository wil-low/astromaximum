#include "PlanetListItem.h"
#include "../utils/DMS.h"
#include "../labels/AstroLabel.h"
#include "../Astronom.h"
#include "../utils/GlyphManager.h"

FXIMPLEMENT(PlanetListItem, FXListItem, NULL, 0)

PlanetListItem::PlanetListItem (AstroLabel* data)
: FXListItem("0", NULL, data)
, deg_mode_(dm_Absolute)
{
}

PlanetListItem::~PlanetListItem(void)
{
}

void PlanetListItem::setDegMode(const FXList* list, deg_mode dm)
{
    deg_mode_ = dm;
	const GlyphManager& gm = GlyphManager::get_const_instance();
	AstroLabel* al = (AstroLabel*)data;
	text[0] = al->getText();
	text[1] = text[2] = "";
	DMS dms;
	switch (dm) {
	    case dm_Absolute:
            dms.calculate(al->getProp(BodyProps::bp_Lon));
            text[1].format("%3d%c%02d\'%02d\"", dms.deg, gm.getDegreeSign(FF_ARIAL), dms.min, dms.sec);
            break;
        case dm_Longitude:
            dms.calculate(al->getProp(BodyProps::bp_Lon));
            text[1].format("%2d%c%02d\'%02d\"", dms.zod_deg, gm.getDegreeSign(FF_ARIAL), dms.min, dms.sec);
            text[2].format("%c", gm.getLabel(TYPE_ZODIAC, dms.zodiac));
            break;
        case dm_RectAsc:
            dms.calculate(al->getProp(BodyProps::bp_RectAsc));
            text[1].format("%3d%c%02d\'%02d\"", dms.deg, gm.getDegreeSign(FF_ARIAL), dms.min, dms.sec);
            break;
        case dm_OblAsc:
            dms.calculate(al->getProp(BodyProps::bp_OblAsc));
            text[1].format("%3d%c%02d\'%02d\"", dms.deg, gm.getDegreeSign(FF_ARIAL), dms.min, dms.sec);
            break;
        case dm_LatDecl:
            dms.calculate(al->getProp(BodyProps::bp_Lat));
            text[1].format("%3d%c%02d\'%02d\"", dms.deg, gm.getDegreeSign(FF_ARIAL), dms.min, dms.sec);
            break;
	}
}

void PlanetListItem::draw(const FXList* list, FXDC& dc, FXint xx, FXint yy, FXint ww, FXint hh) const
{
    const int SIDE_MARGIN = 5;
	FXFont *font = list->getFont();
	FXint ih=0, th = font->getFontHeight();
	FXFont* afont = GlyphManager::get_const_instance().getFont(12, FF_ARIAL);
	FXint ath = afont->getFontHeight();

	if(isSelected())
		dc.setForeground(list->getSelBackColor());
	else
		dc.setForeground(list->getBackColor());     // FIXME maybe paint background in onPaint?
	dc.fillRectangle(xx, yy, ww, hh);
	if (hasFocus()){
		dc.drawFocusRectangle(xx + 1, yy + 1, ww - 2, hh - 2);
	}
	if (!isEnabled())
		dc.setForeground(makeShadowColor(list->getBackColor()));
	else if (isSelected())
		dc.setForeground(list->getSelTextColor());
	else
		dc.setForeground(list->getTextColor());

	AstroLabel* al = (AstroLabel*)data;
	if (al->getType() == TYPE_HOUSE && al->getFlags() == hf_Undef)
		dc.setFont(afont);
	dc.drawText(xx + SIDE_MARGIN, yy + (hh - th) / 2 + dc.getFont()->getFontAscent(), text[0]);
	dc.setFont(font);

	if (!text[1].empty()) {
	    int ofs = afont->getTextWidth(text[1]) + SIDE_MARGIN + 30;
		dc.setFont(afont);
        dc.drawText(xx + ww - ofs, yy + (hh - th) / 2 + dc.getFont()->getFontAscent(), text[1]);
		dc.setFont(font);
	}

	if (!text[2].empty()) {
	    int ofs = font->getTextWidth(text[2]) + SIDE_MARGIN;
        dc.drawText(xx + ww - ofs, yy + (hh - th) / 2 + dc.getFont()->getFontAscent(), text[2]);
	}

  /*
	char cc; AnsiString str;
	dc.Font=AFnt; dc.Font->Size--;//-=2;
	TWidget* pp=(TWidget*)(((TListBox *)Control)->Items->Objects[Index]);
	Planet plt;
	AnsiString dms,charm,bidd;
	int dzod=((TListBox *)Control)->Width-60;
	int ddeg=aster? 110 : 35;

	bool decls; double d_abs;
	pp->body->GetDMS(plt);
	int old_deg=plt.deg;
	dms="%3d\xb0%02d\'%02d\"";
	switch(deg_mode){
	case 1:
		dms="%02d\xb0%02d\'";
		if(!aster){
			if((pp->body->id>=101)&&(pp->body->id<=112)){
				ddeg=40;
			}
			else{
				ddeg=25;
				dms="%02d\xb0%02d\'%02d\"";
			}
		}
		cc=activework->lZodChars[plt.zodiac];
		dc.Font=AFnt;
		dc.Font->Color=clBlack;
		dc.Font->Size=12;
		dc.TextOut(Rect.Left+5+dzod, Rect.Top+2,cc);
		plt.deg=plt.zod_deg;
		break;
	case 2:
		pp->body->GetDMS(plt,pp->body->rectasc);
		break;
	case 3:
		pp->body->GetDMS(plt,pp->body->oblasc);
		break;
	}
	if(deg_mode<=1){
		dc.Font->Color=CC_GRAY;
		dc.Font->Size=10;
		dc.TextOut(Rect.Left + aster? 172: 122, Rect.Top+4,
			str.sprintf("%c%c",dp->GetSymbol(GetGradarc2(pp->plt.deg)),
			dp->GetSymbol(GetGradarc(pp->plt.deg))));
	}
	if(deg_mode==4){
		double aa[2]={pp->body->data[1],pp->body->decl};
		AnsiString ss; ddeg-=8;
		for(int i=0; i<2; i++){
			decls=aa[i]>=0;
			d_abs=fabs(aa[i]);
			pp->body->GetDMS(plt,d_abs);
			str+=ss.sprintf("%2d%c%02d ",plt.deg,decls?'n':'s',plt.min);
		}
	}
	else
		str.sprintf(dms.c_str(),plt.deg,plt.min,plt.sec);
	dc.Font=OFnt; dc.Font->Size--;//-=2;
	dc.Font->Color=CC_GRAY;
	if(deg_mode<=1){
		for(int i=0;i<7;i++)
			if(old_deg==degarray[0][i]) dc.Font->Color=clWhite;
		for(int i=0;i<7;i++)
			if(old_deg==degarray[1][i]) dc.Font->Color=clBlack;
	}
	dc.TextOut(Rect.Left + ddeg, Rect.Top+2,str);

	if(aster){
		char pl_name[255];
		swe_get_planet_name(pp->body->id, pl_name);
		if(strcmp(pl_name,"name not found")==0){
			AnsiString sss=dp->ReadString("names",IntToStr(pp->body->id),"");
			strcpy(pl_name,sss.c_str());
		}
		bidd=pl_name; dc.Font=OFnt;
		int vv=pp->Visible? 1: 0;
		dc.Font->Color = lstFontColor[vv][pp->pChart->id];
	}
	else{
		bidd=dp->GetSymbol(pp->body->id);
		dc.Font=AFnt;
		dc.Font->Color = lstFontColor[0][pp->pChart->id];
	}
	dc.Font->Size--;//-=2;
	int hn=(eqh_planet==-1)? 0: 1;
	if((pp->body->id>=101)&&(pp->body->id<=112))
		bidd=house_names[hn][pp->body->id-101];
	dc.TextOut(Rect.Left + 5, Rect.Top+2,bidd);
	ddeg=aster? 96 : 17;
	const double STABLESPEED=6./60/60;
	if((pp->body->id<100)||(pp->body->id>200)||(pp->body->id==148)){
		if(pp->body->data[3]<-STABLESPEED){
			dc.Font=AFnt;
			dc.Font->Size=6;
			dc.TextOut(Rect.Left + ddeg, Rect.Top+10,"R");
		}
		if(!fChrono->cboDynam->ItemIndex&&(fabs(pp->body->data[3])<STABLESPEED)){
			dc.Font->Name="Arial";
			dc.Font->Size=7;
			dc.TextOut(Rect.Left + ddeg, Rect.Top+6,"s");
		}
	}
*/
}
