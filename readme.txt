使用方法：
	在对应的对话框类中的OnInitDialog()中调用InitLangDialog(this,IDD_DLG_XXX);
	IDD_DLG_XXX为对话框的ID

作用：
	对话框运行时从Lang.ini获取其所有可得到的字符串，如果无法获得字符串以本身字符串默认并保存到		Lang.ini语言文件中，每个控件用对话框ID和控件ID作为唯一标识。
	以此达到在配置文件Lang.ini统一给控件命名，如中英文切换等。
