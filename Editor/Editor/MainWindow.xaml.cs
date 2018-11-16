using System;
using System.Collections.Generic;
//using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Xml;
using S16.Drawing;

namespace Editor {


	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window {

		public static String gameDirectory = AppDomain.CurrentDomain.BaseDirectory + "../../../../";
		public static String gfxDir = gameDirectory + "assets/gfx/";

		public static String assetManifestFile = gameDirectory + "assets/AssetManifest.xml";
		public static String levelManifestFile = gameDirectory + "assets/LevelManifest.xml";

		public static XmlDocument assetManifestDoc;

		XmlDocument levelManifestDoc;
		XmlNode levelManifestRoot;
		List<LevelData> levelDatas = new List<LevelData>();


		public MainWindow() {
			InitializeComponent();

			assetManifestDoc = new XmlDocument();
			assetManifestDoc.Load(assetManifestFile);





			// get level info from xml
			levelManifestDoc = new XmlDocument();
			levelManifestDoc.Load(levelManifestFile);
			levelManifestRoot = levelManifestDoc.GetElementsByTagName("root")[0];
			int i = 0;
			foreach (XmlNode node in levelManifestRoot.ChildNodes) {
				if (node.Name == "level") {
					LevelData ld = new LevelData(node);
					Image img = new Image();
					img.Source = ld.previewImage;
					previewStack.Children.Add(img);

					levelDatas.Add(ld);
				}
			}

		}
	}


	public class LevelData {

		[System.Runtime.InteropServices.DllImport("gdi32.dll")]
		public static extern bool DeleteObject(IntPtr hObject);

		private String levelXmlFile;
		private String name;


		public BitmapSource previewImage;

		public LevelData(XmlNode node) {

			name = node.Attributes["name"].Value;
			levelXmlFile = node.Attributes["file"].Value;
			String previewImgName = node.Attributes["preview"].Value;
			XmlNode assetNode = MainWindow.assetManifestDoc.SelectNodes("//sprite[@name='" + previewImgName + "']")[0];
			String previewImageFile = assetNode.Attributes["file"].Value;

			DDSImage previewDDS = new DDSImage(
					File.ReadAllBytes(MainWindow.gfxDir + previewImageFile));

			var hbitmap = previewDDS.BitmapImage.GetHbitmap();
			previewImage = System.Windows.Interop.Imaging.CreateBitmapSourceFromHBitmap(
				hbitmap, IntPtr.Zero, System.Windows.Int32Rect.Empty,
				System.Windows.Media.Imaging.BitmapSizeOptions.FromEmptyOptions());

			DeleteObject(hbitmap);
		}

	}
}
