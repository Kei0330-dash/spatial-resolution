// vertical frame without border and title
TGVButtonGroup *bg = new TGVButtonGroup(main_frame);
 
// create text button with id=1
TGTextButton *button1 = new TGTextButton(bg,"some text");
 
// create another text button with id=2
TGTextButton *button2 = new TGTextButton(bg,"another text");
 
// map all buttons
bg->Show();