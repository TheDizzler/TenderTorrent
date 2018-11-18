using System;
using System.Collections.Generic;
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

namespace Editor {
	/// <summary>
	/// Interaction logic for LevelEditorPage.xaml
	/// </summary>
	public partial class LevelEditorPage : Page {

		private LevelData levelData;
		private XmlNode levelRoot;
		private XmlNode waypointsNode;

		private String assetDir;
		private Single numLayers;
		private Single scale;
		/// <summary>
		/// Layer 0.
		/// </summary>
		private String bgFile;
		private String bgSpriteSheet;
		private List<BgLayerSet> bgLayers = new List<BgLayerSet>();


		public LevelEditorPage(LevelData lvlData) {
			InitializeComponent();
			levelData = lvlData;

			parseLevelData();

			// populate the Editor
			BitmapSource bgBitmap = levelData.parseDDSFile(bgFile);
			bgImage.Source = bgBitmap;

			foreach (BgLayerSet layerSet in bgLayers) {
				TreeViewItem layerItem = new TreeViewItem { Header = layerSet.set };
				foreach (Cloth cloth in layerSet.pieces) {
					layerItem.Items.Add(new TreeViewItem { Header = cloth.part });
				}
				int i = 0;
				foreach (SubLayer sub in layerSet.subLayers) {
					TreeViewItem subItem = new TreeViewItem { Header = "Sub Layer " + i };
					foreach (Cloth cloth in sub.pieces) {
						subItem.Items.Add(new TreeViewItem { Header = cloth.part });
					}
					layerItem.Items.Add(subItem);
				}
				layerTreeview.Items.Add(layerItem);
			}
		}


		private void parseLevelData() {

			// open level xml
			XmlDocument levelDoc = levelData.loadLevel();
			levelRoot = levelDoc.GetElementsByTagName("level")[0];

			levelNameLabel.Content = levelRoot.Attributes["name"];
			bool result = float.TryParse(levelRoot.Attributes["scale"].Value, out scale);
			result = float.TryParse(levelRoot.Attributes["numLayers"].Value, out numLayers);
			assetDir = levelRoot.Attributes["dir"].Value;

			waypointsNode = levelRoot.SelectNodes("waypoints")[0];

			bgFile = assetDir + levelRoot.SelectNodes("base")[0].InnerText;
			bgSpriteSheet = levelRoot.SelectNodes("backgroundSpriteSheet")[0].InnerText;


			// parse bgLayer pieces
			foreach (XmlNode bgLayerNode in levelRoot.SelectNodes("backgroundLayer")) {
				bgLayers.Add(new BgLayerSet(bgLayerNode));
			}
		}

		private class BgLayerSet {

			public XmlNode layerNode;
			public String set;

			public List<Cloth> pieces = new List<Cloth>();
			public List<SubLayer> subLayers = new List<SubLayer>();


			public BgLayerSet(XmlNode node) {

				layerNode = node;
				set = layerNode.Attributes["set"].Value;


				foreach (XmlNode clothNode in layerNode.SelectNodes("cloth")) {
					pieces.Add(new Cloth(clothNode));
				}
				foreach (XmlNode subLayerNode in layerNode.SelectNodes("subLayer")) {
					subLayers.Add(new SubLayer(subLayerNode));
				}
			}
		}

		private class Cloth {

			public String part;

			XmlNode clothNode;

			Vector positionInSheet;
			Vector size;
			Vector position;

			int health;

			public List<Tatter> tatters = new List<Tatter>();


			public Cloth(XmlNode node) {

				clothNode = node;
				if (clothNode.Attributes["part"] == null) {
					part = "Cloth";
				} else {
					part = clothNode.Attributes["part"].Value;
				}
				int temp;
				Boolean result = Int32.TryParse(clothNode.Attributes["x"].Value, out temp);
				positionInSheet.X = temp;
				result = Int32.TryParse(clothNode.Attributes["y"].Value, out temp);
				positionInSheet.Y = temp;
				result = Int32.TryParse(clothNode.Attributes["health"].Value, out temp);
				health = temp;

				XmlNode posNode = clothNode.SelectSingleNode("position");
				result = Int32.TryParse(posNode.Attributes["x"].Value, out temp);
				position.X = temp;
				result = Int32.TryParse(posNode.Attributes["y"].Value, out temp);
				position.Y = temp;

				XmlNode sizeNode = clothNode.SelectSingleNode("size");
				result = Int32.TryParse(sizeNode.Attributes["x"].Value, out temp);
				size.X = temp;
				result = Int32.TryParse(sizeNode.Attributes["y"].Value, out temp);
				size.Y = temp;

				foreach (XmlNode tatter in clothNode.SelectNodes("tatter")) {
					tatters.Add(new Tatter(tatter));
				}
			}

		}

		private class Tatter {
			XmlNode tatterNode;
			Vector positionInSheet;
			Vector size;

			public Tatter(XmlNode node) {
				tatterNode = node;
				int pos;
				Boolean result = Int32.TryParse(tatterNode.Attributes["x"].Value, out pos);
				positionInSheet.X = pos;
				result = Int32.TryParse(tatterNode.Attributes["y"].Value, out pos);
				positionInSheet.Y = pos;

				if (tatterNode.HasChildNodes) {
					XmlNode sizeNode = tatterNode.ChildNodes[0];
					result = Int32.TryParse(sizeNode.Attributes["x"].Value, out pos);
					size.X = pos;
					result = Int32.TryParse(sizeNode.Attributes["y"].Value, out pos);
					size.Y = pos;
				}
			}
		}

		private class SubLayer {

			XmlNode layerNode;
			public List<Cloth> pieces = new List<Cloth>();

			public SubLayer(XmlNode node) {

				layerNode = node;
				foreach (XmlNode clothNode in layerNode.ChildNodes) {
					pieces.Add(new Cloth(clothNode));
				}
			}

		}
	}
}
