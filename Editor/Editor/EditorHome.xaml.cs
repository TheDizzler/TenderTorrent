using System;
using System.Collections.Generic;
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
	/// Interaction logic for EditorHome.xaml
	/// </summary>
	public partial class EditorHome : Page {

		public static String gameDirectory = AppDomain.CurrentDomain.BaseDirectory + "../../../../";
		public static String assetDir = gameDirectory + "assets/";
		public static String gfxDir = assetDir + "gfx/";

		public static String assetManifestFile = assetDir + "AssetManifest.xml";
		public static String levelManifestFile = assetDir + "LevelManifest.xml";

		public static XmlDocument assetManifestDoc;

		XmlDocument levelManifestDoc;
		XmlNode levelManifestRoot;
		List<LevelButton> levelButtons = new List<LevelButton>();

		public EditorHome() {
			InitializeComponent();

			assetManifestDoc = new XmlDocument();
			assetManifestDoc.Load(assetManifestFile);

			// get level info from xml
			levelManifestDoc = new XmlDocument();
			levelManifestDoc.Load(levelManifestFile);
			levelManifestRoot = levelManifestDoc.GetElementsByTagName("root")[0];

			foreach (XmlNode node in levelManifestRoot.ChildNodes) {
				if (node.Name == "level") {

					LevelButton lvlBtn = new LevelButton(node);
					lvlBtn.PreviewMouseDown += OpenLevel;
					previewPanel.Children.Add(lvlBtn);
					levelButtons.Add(lvlBtn);
				}
			}
		}


		private class LevelButton : Button {

			public LevelData levelData;

			public LevelButton(XmlNode node) {
				levelData = new LevelData(node);
				
				Width = 120;
				Height = 153;
				Margin = new Thickness(10, 10, 0, 0);
				BorderThickness = new Thickness(10, 10, 10, 10);
				
				Image img = new Image();
				img.Source = levelData.previewImage;

				Label label = new Label();
				label.Content = levelData.name;
				label.VerticalAlignment = VerticalAlignment.Bottom;
				label.HorizontalAlignment = HorizontalAlignment.Center;
				label.Background = Brushes.Aqua;
				label.Foreground = Brushes.Red;

				Grid grid = new Grid();
				grid.Children.Add(img);
				grid.Children.Add(label);
				ContentControl cc = new ContentControl();
				cc.Width = 100;
				cc.Height = 133;
				cc.Content = grid;
				Content = cc;
			}

		}

		private void OpenLevel(Object sender, MouseButtonEventArgs e) {

			LevelButton lvlBtn = (LevelButton) sender;
			LevelEditorPage levelEditor = new LevelEditorPage(lvlBtn.levelData);
			NavigationService.Navigate(levelEditor);
		}

	}


	public class LevelData {

		[System.Runtime.InteropServices.DllImport("gdi32.dll")]
		public static extern bool DeleteObject(IntPtr hObject);

		private String levelXmlFile;
		public String name;

		private XmlDocument levelDoc;

		public BitmapSource previewImage;


		public LevelData(XmlNode node) {

			name = node.Attributes["name"].Value;
			levelXmlFile = node.Attributes["file"].Value;
			String previewImgName = node.Attributes["preview"].Value;
			XmlNode assetNode = EditorHome.assetManifestDoc.SelectNodes("//sprite[@name='" + previewImgName + "']")[0];
			String previewImageFile = assetNode.Attributes["file"].Value;

			DDSImage previewDDS = new DDSImage(
					File.ReadAllBytes(EditorHome.gfxDir + previewImageFile));

			var hbitmap = previewDDS.BitmapImage.GetHbitmap();
			previewImage = System.Windows.Interop.Imaging.CreateBitmapSourceFromHBitmap(
				hbitmap, IntPtr.Zero, System.Windows.Int32Rect.Empty,
				System.Windows.Media.Imaging.BitmapSizeOptions.FromEmptyOptions());

			DeleteObject(hbitmap);
		}


		public XmlDocument loadLevel() {
			levelDoc = new XmlDocument();
			levelDoc.Load(EditorHome.assetDir + levelXmlFile);
			return levelDoc;
		}


		public BitmapSource parseDDSFile(String filepath) {

			DDSImage imgDDS = new DDSImage(
					File.ReadAllBytes(EditorHome.gameDirectory + filepath));

			var hbitmap = imgDDS.BitmapImage.GetHbitmap();
			BitmapSource bitsource = System.Windows.Interop.Imaging.CreateBitmapSourceFromHBitmap(
				hbitmap, IntPtr.Zero, System.Windows.Int32Rect.Empty,
				System.Windows.Media.Imaging.BitmapSizeOptions.FromEmptyOptions());

			DeleteObject(hbitmap);

			return bitsource;
		}

	}
}
