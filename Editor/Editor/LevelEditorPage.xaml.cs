using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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

namespace Editor {
	/// <summary>
	/// Interaction logic for LevelEditorPage.xaml
	/// </summary>
	public partial class LevelEditorPage : Page {

		private LevelData levelData;
		private XmlNode levelRoot;
		private XmlNode waypointsNode;

		public static String LEVEL_ASSET_DIR;
		private int numLayers;
		private float scale = 1;

		private String bgSpriteSheet;
		private List<GameObject> bgLayers = new List<GameObject>();
		private const String outputFileName = @"D:\github projects\FirstGame\Editor\TempImage.png";
		public BitmapSource spriteSheetBitmapSource;


		public LevelEditorPage(LevelData lvlData) {
			InitializeComponent();
			levelData = lvlData;

			parseLevelData();

			// populate the Editor
			buildLayerTree();

			createLevelBg(null);

		}

		private void buildLayerTree() {

			layerTreeview.Items.Add(bgLayers[0]);

			for (int i = 1; i < bgLayers.Count; ++i) {

				BgLayerSet layerSet = (BgLayerSet)bgLayers[i];

				StackPanel sp = new StackPanel();
				sp.Orientation = Orientation.Horizontal;
				CheckBox cb = new CheckBox();
				cb.IsChecked = true;
				cb.Click += layerCheckBox;
				sp.Children.Add(cb);

				TreeViewItem layerItem = new TreeViewItem { Header = layerSet.set };


				foreach (Cloth cloth in layerSet.pieces) {
					StackPanel spChild = new StackPanel();
					spChild.Orientation = Orientation.Horizontal;
					CheckBox cbChild = new CheckBox();
					cbChild.IsChecked = true;
					cbChild.Click += layerCheckBox;
					spChild.Children.Add(cbChild);
					spChild.Children.Add(new TreeViewItem { Header = cloth.part });
					layerItem.Items.Add(spChild);
				}
				int subNum = 0;
				foreach (SubLayer sub in layerSet.subLayers) {
					StackPanel spChild = new StackPanel();
					spChild.Orientation = Orientation.Horizontal;
					CheckBox cbChild = new CheckBox();
					cbChild.IsChecked = true;
					cbChild.Click += layerCheckBox;
					spChild.Children.Add(cbChild);
					TreeViewItem subItem = new TreeViewItem { Header = "Sub Layer " + subNum };
					foreach (Cloth cloth in sub.pieces) {
						StackPanel spSubChild = new StackPanel();
						spSubChild.Orientation = Orientation.Horizontal;
						CheckBox cbSubChild = new CheckBox();
						cbSubChild.IsChecked = true;
						cbSubChild.Click += layerCheckBox;
						spSubChild.Children.Add(cbSubChild);
						spSubChild.Children.Add(new TreeViewItem { Header = cloth.part });

						subItem.Items.Add(spSubChild);
					}
					spChild.Children.Add(subItem);
					layerItem.Items.Add(spChild);
				}

				sp.Children.Add(layerItem);
				layerTreeview.Items.Add(sp);
			}


		}

		private void layerCheckBox(Object sender, RoutedEventArgs e) {

			// show / hide
		}

		private void parseLevelData() {

			// open level xml
			XmlDocument levelDoc = levelData.loadLevel();
			levelRoot = levelDoc.GetElementsByTagName("level")[0];

			levelNameLabel.Content = levelRoot.Attributes["name"];

			bool result = float.TryParse(levelRoot.Attributes["scale"].Value, out scale);
			result = Int32.TryParse(levelRoot.Attributes["numLayers"].Value, out numLayers);
			LEVEL_ASSET_DIR = EditorHome.GAME_DIRECTORY + levelRoot.Attributes["dir"].Value;

			waypointsNode = levelRoot.SelectNodes("waypoints")[0];

			bgLayers.Add(new BgImage(levelRoot.SelectNodes("base")[0]));

			bgSpriteSheet = levelRoot.SelectNodes("backgroundSpriteSheet")[0].InnerText;
			spriteSheetBitmapSource = LevelData.parseDDSFile(LEVEL_ASSET_DIR + bgSpriteSheet);
			spriteSheetImage.Source = spriteSheetBitmapSource;

			// parse bgLayer pieces
			foreach (XmlNode bgLayerNode in levelRoot.SelectNodes("backgroundLayer")) {
				bgLayers.Add(new BgLayerSet(bgLayerNode, spriteSheetBitmapSource));
			}
		}

		public void createLevelBg(CheckBox[] layerCheckBoxes) {

			int width = bgLayers[0].bitmap.PixelWidth;
			int height = bgLayers[0].bitmap.PixelHeight;


			DrawingVisual dv = new DrawingVisual();
			using (DrawingContext dc = dv.RenderOpen()) {
				bgLayers[0].addBitmapSource(dc);
				for (int i = 1; i < bgLayers.Count; ++i) {

					BgLayerSet layer = (BgLayerSet)bgLayers[i];
					foreach (Cloth cloth in layer.pieces) {
						cloth.addBitmapSource(dc);
					}
					foreach (SubLayer sub in layer.subLayers) {
						foreach (Cloth cloth in sub.pieces) {
							cloth.addBitmapSource(dc);
						}
					}
				}
			}

			RenderTargetBitmap bmp = new RenderTargetBitmap(
				width, height, 96, 96, PixelFormats.Pbgra32);
			bmp.Render(dv);

			PngBitmapEncoder encoder = new PngBitmapEncoder();
			encoder.Frames.Add(BitmapFrame.Create(bmp));

			using (Stream stream = File.Create(outputFileName)) {
				encoder.Save(stream);
			}

			bgImage.Source = new BitmapImage(new Uri(outputFileName, UriKind.Absolute));
		}

		private void OnTreeViewItemChanged(Object sender, RoutedPropertyChangedEventArgs<Object> e) {

		}
	}


	public abstract class GameObject : TreeViewItem {
		protected XmlNode node;
		public BitmapSource bitmap;

		public GameObject(XmlNode objectNode) {
			node = objectNode;

		}

		public abstract void addBitmapSource(DrawingContext dc);
	}

	/// <summary>
	/// Layer 0.
	/// </summary>
	public class BgImage : GameObject {

		private String bgFile;

		public BgImage(XmlNode node) : base(node) {

			bgFile = LevelEditorPage.LEVEL_ASSET_DIR + node.InnerText;
			bitmap = LevelData.parseDDSFile(bgFile);
		}

		public override void addBitmapSource(DrawingContext dc) {
			dc.DrawImage(bitmap, new Rect(0, 0, bitmap.PixelWidth, bitmap.PixelHeight));
		}
	}


	public class BgLayerSet : GameObject {

		public new String Header { get { return set; } set { } }

		public String set;

		public ObservableCollection<Cloth> pieces { get; set; }
		public ObservableCollection<SubLayer> subLayers { get; set; }


		public BgLayerSet(XmlNode node, BitmapSource spriteSheetBitmapSource) : base(node) {

			pieces = new ObservableCollection<Cloth>();
			subLayers = new ObservableCollection<SubLayer>();
			set = node.Attributes["set"].Value;

			foreach (XmlNode clothNode in node.SelectNodes("cloth")) {
				pieces.Add(new Cloth(clothNode, spriteSheetBitmapSource));
			}
			foreach (XmlNode subLayerNode in node.SelectNodes("subLayer")) {
				subLayers.Add(new SubLayer(subLayerNode, spriteSheetBitmapSource));
			}
		}

		public override void addBitmapSource(DrawingContext dc) {
			return;
		}

	}

	public class Cloth : GameObject {
		public new String Header { get { return part; } set { } }
		public String part;
		public Point position;
		public int health;

		Point positionInSheet;
		Point size;


		public List<Tatter> tatters { get; set; }


		public Cloth(XmlNode node, BitmapSource spriteSheetBitmapSource) : base(node) {

			tatters = new List<Tatter>();

			if (node.Attributes["part"] == null) {
				part = "Cloth";
			} else {
				part = node.Attributes["part"].Value;
			}
			int temp;
			Boolean result = Int32.TryParse(node.Attributes["x"].Value, out temp);
			positionInSheet.X = temp;
			result = Int32.TryParse(node.Attributes["y"].Value, out temp);
			positionInSheet.Y = temp;
			result = Int32.TryParse(node.Attributes["health"].Value, out temp);
			health = temp;

			XmlNode posNode = node.SelectSingleNode("position");
			result = Int32.TryParse(posNode.Attributes["x"].Value, out temp);
			position.X = temp;
			result = Int32.TryParse(posNode.Attributes["y"].Value, out temp);
			position.Y = temp;

			XmlNode sizeNode = node.SelectSingleNode("size");
			result = Int32.TryParse(sizeNode.Attributes["x"].Value, out temp);
			size.X = temp;
			result = Int32.TryParse(sizeNode.Attributes["y"].Value, out temp);
			size.Y = temp;

			bitmap = new CroppedBitmap(spriteSheetBitmapSource,
				new Int32Rect((int)positionInSheet.X, (int)positionInSheet.Y, (int)size.X, (int)size.Y));

			foreach (XmlNode tatter in node.SelectNodes("tatter")) {
				tatters.Add(new Tatter(tatter, spriteSheetBitmapSource));
			}
		}

		public override void addBitmapSource(DrawingContext dc) {
			dc.DrawImage(bitmap, new Rect(position.X, position.Y, size.X, size.Y));
		}
	}


	public class SubLayer : GameObject {

		public List<Cloth> pieces { get; set; }

		public SubLayer(XmlNode subnode, BitmapSource spriteSheetBitmapSourc) : base(subnode) {

			pieces = new List<Cloth>();

			foreach (XmlNode clothNode in node.ChildNodes) {
				pieces.Add(new Cloth(clothNode, spriteSheetBitmapSourc));
			}
		}

		public override void addBitmapSource(DrawingContext dc) {
			return;
		}
	}


	public class Tatter {
		XmlNode tatterNode;
		Point positionInSheet;
		Point size;

		BitmapSource bitmap;


		public Tatter(XmlNode node, BitmapSource spriteSheetBitmapSource) {
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
			} else {
				XmlNode sizeNode = tatterNode.ParentNode.SelectSingleNode("size");
				result = Int32.TryParse(sizeNode.Attributes["x"].Value, out pos);
				size.X = pos;
				result = Int32.TryParse(sizeNode.Attributes["y"].Value, out pos);
				size.Y = pos;
			}

			bitmap = new CroppedBitmap(spriteSheetBitmapSource,
				new Int32Rect((int)positionInSheet.X, (int)positionInSheet.Y, (int)size.X, (int)size.Y));
		}

		public void getBitmapSource(DrawingContext dc) {
			dc.DrawImage(bitmap, new Rect(0, 0, size.X, size.Y));
		}
	}
}
