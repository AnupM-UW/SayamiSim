// #define TEST

using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using Newtonsoft.Json;


namespace SimulatorListenerUDP
{
#if TEST

    public class TestData
    {
        public double lat { get; set; }

        [JsonProperty(PropertyName = "long")]
        public double lng { get; set; }

        public double alt { get; set; }

        public double hdg { get; set; }

        public double aoa { get; set; }

        public double timestamp { get; set; }
    }
#endif

    public class UDPMulticastClient
    {
        // derived from:
        // http://www.jarloo.com/c-udp-multicasting-tutorial/
        // 

        // Note that the router has to support multicast over UDP. You can enable it from QoS settings for NetGear
        // routers. My router did not support it, so I cannot use UDP over multicast, and I have to use UDP P2P
        // see:
        //   https://www.techwalla.com/articles/how-to-enable-multicast-upnp-technology-on-a-netgear-wndr3300-router
        //
        private const string UPDMULTICASTGROUP = "224.255.1.15"; // agreed upon IP address
        private const int PORT = 54320;   // agreed upon port

        private UdpClient client;
        private IPEndPoint localEndPt;

        public UDPMulticastClient() : this(UPDMULTICASTGROUP, PORT)
        {
        }

        public UDPMulticastClient(string multicastGroupAddress, int port) // throws FormatException
        {
            localEndPt = new IPEndPoint(IPAddress.Any, port);

            client = new UdpClient();
            client.ExclusiveAddressUse = false;
            client.Client.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
            client.Client.Bind(localEndPt);  // bind the socket to this port

            IPAddress multicastAddress;

            multicastAddress = IPAddress.Parse(multicastGroupAddress);

            client.JoinMulticastGroup(multicastAddress);

        }

        public async Task<string> GetDataAsync()
        {
            string result = "";
#if TEST
            TestData dat = new TestData { lat = 3.4, lng = 5.5, alt = 10000, hdg = 125.23, aoa = 5.5, timestamp = (double)DateTime.Now.Millisecond };
            string sJson = JsonConvert.SerializeObject(dat);
            await Task.Run(() => { result = sJson; });
            return result;
#else
            UdpReceiveResult res;

            try
            {
                res = await client.ReceiveAsync();
            }
            catch (SocketException e)
            {
                return "Error getting data from socket: " + e.Message;
            }
            return Encoding.UTF8.GetString(res.Buffer);
#endif
        }

        public string GetData()
        {
            byte[] val;
            try
            {
                IPEndPoint remoteEndPt = new IPEndPoint(IPAddress.Any, localEndPt.Port);
                val = client.Receive(ref remoteEndPt);
            }
            catch (SocketException e)
            {
                return "Error getting data from socket: " + e.Message;
            }
            return Encoding.UTF8.GetString(val);
        }

        private void Err(string msg)
        {
            // Application.Current.MainWindow
        }

    }

    public class UDPP2PClient
    {
        // Note that the router has to support multicast over UDP. You can enable it from QoS settings for NetGear
        // routers. My router did not support it, so I cannot use UDP over multicast, and I have to use UDP P2P
        // see:
        //   https://www.techwalla.com/articles/how-to-enable-multicast-upnp-technology-on-a-netgear-wndr3300-router
        //
        private const string PEER_ADDRESS = "192.168.1.26"; // agreed upon IP address
        private const int PORT = 54320;   // agreed upon port

        private UdpClient client;
        private IPEndPoint localEndPt;

        public UDPP2PClient() : this(PEER_ADDRESS, PORT) { }

        public UDPP2PClient(string peerAddr) : this(peerAddr, PORT)
        {
        }

        public UDPP2PClient(string peerAddr, int port) // throws FormatException
        {
            localEndPt = new IPEndPoint(IPAddress.Any, port); // use any available IP address on any NIC

            client = new UdpClient();
            client.ExclusiveAddressUse = false;
            client.Client.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
            client.Client.Bind(localEndPt);  // bind the socket to this port

            IPAddress peerIp = IPAddress.Parse(peerAddr);

            // client.JoinMulticastGroup(multicastAddress);

        }


        public async Task<string> GetDataAsync()
        {
            string result = "";
#if TEST
            TestData dat = new TestData { lat = 3.4, lng = 5.5, alt = 10000, hdg = 125.23, aoa = 5.5, timestamp = (double)DateTime.Now.Millisecond };
            string sJson = JsonConvert.SerializeObject(dat);
            await Task.Run(() => { result = sJson; });
            return result;
#else
            UdpReceiveResult res;

            try
            {
                res = await client.ReceiveAsync();
            }
            catch (SocketException e)
            {
                return "Error getting data from socket: " + e.Message;
            }
            result = Encoding.UTF8.GetString(res.Buffer);
            return result;
#endif
        }

        public string GetData()
        {
            byte[] val;
            try
            {
                // Create an IPEndPoint to receive the IP Address and port number of the sender. 
                // The IPEndPoint will allow you to read datagrams sent from any source.
                IPEndPoint remoteEndPt = new IPEndPoint(IPAddress.Any, localEndPt.Port);
                val = client.Receive(ref remoteEndPt);
            }
            catch (SocketException e)
            {
                return "Error getting data from socket: " + e.Message;
            }
            return Encoding.UTF8.GetString(val);
        }

    }
}
