using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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
using System.Windows.Threading;
using Newtonsoft.Json;

namespace SimulatorListenerUDP
{
    public class SimulatorData
    {
        public double lat { get; set; }

        [JsonProperty(PropertyName ="long")]
        public double lng { get; set; }

        public double alt { get; set; }

        public double hdg { get; set; }

        public double aoa { get; set; }

        public double timestamp { get; set; }

    }

    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private UDPP2PClient udpClient;
        // private UDPMulticastClient udpClient;
        Queue<SimulatorData> simulatorData;
        int queueSize = 15;

        public MainWindow()
        {
            InitializeComponent();
            udpClient = new UDPP2PClient();
            simulatorData = new Queue<SimulatorData>(5);
            this.Loaded += MainWindow_Loaded;

        }

        private void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {
            lbGetInfo.Items.Add("Loaded");

            DispatcherTimer timer = new DispatcherTimer(TimeSpan.FromSeconds(2), DispatcherPriority.Normal, TimeHandler, Dispatcher.CurrentDispatcher);
            
        }

        private async void TimeHandler(object sender, EventArgs args)
        {
            
            // lbGetInfo.Items.Add("Timer");
            string val = await udpClient.GetDataAsync(); // .GetAwaiter().GetResult();
            SimulatorData data = JsonConvert.DeserializeObject<SimulatorData>(val);
            while(simulatorData.Count >= queueSize)
            {
                // get it to right size
                simulatorData.Dequeue();
            }
            simulatorData.Enqueue(data);

            lbGetInfo.Items.Clear();
            List<SimulatorData> ls = simulatorData.ToList();
            foreach (var v in ls)
            {
                lbGetInfo.Items.Add($"Timestamp: {v.timestamp} Alt: {v.alt} Hdg: {v.hdg} AoA: {v.aoa}");
            }
            RotateTransform imgTransform = imgCompass.LayoutTransform as RotateTransform;
            double heading = data.hdg;

            imgTransform.Angle = -heading;
        }

    }
}
