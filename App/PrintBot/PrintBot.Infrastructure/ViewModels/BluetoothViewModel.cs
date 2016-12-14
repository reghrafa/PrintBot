using Plugin.BLE.Abstractions.Contracts;
using PrintBot.Domain.Api.Clients;
using PrintBot.Domain.RequiredInterfaces;
using PrintBot.Infrastructure.Mvvm;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PrintBot.Infrastructure.ViewModels
{
    public class BluetoothViewModel : ViewModelBase
    {
        private IBluetoothClient _client;
        private ObservableCollection<IDevice> _pairedDevices;
        public ObservableCollection<IDevice> PairedDevices
        {
            get { return _pairedDevices ?? (_pairedDevices = new ObservableCollection<IDevice>()); }
            set
            {
                if (_pairedDevices != value) _pairedDevices = value;
            }
        }
        private ObservableCollection<IDevice> _foundDevices;
        public ObservableCollection<IDevice> FoundDevices
        {
            get { return _foundDevices ?? (_foundDevices = new ObservableCollection<IDevice>()); }
            set
            {
                if (_foundDevices != value) _foundDevices = value;
            }
        }

        // Constants
        private const string NAME_OF_SERVICE = "TI SensorTag Smart Keys";
        private const string NAME_OF_CHARACTERISTIC = "TI SensorTag Keys Data";

        // State of the connection
        private ConnectionState _connectionStatus;
        public ConnectionState ConnectionStatus
        {
            get { return _connectionStatus; }
            set
            {
                if (_connectionStatus != value)
                {
                    _connectionStatus = value;
                    OnPropertyChanged("ConnectionStatus");
                }
            }
        }

        private IDevice _connectedDevice;
        public IDevice ConnectedDevice
        {
            get
            {
                return _connectedDevice;
            }
            set
            {
                if (_connectedDevice != value) _connectedDevice = value;
            }
        }

        private bool _connected;
        public bool Connected
        {
            get { return _connected; }
            set
            {
                if (_connected != value) _connected = value;
                OnPropertyChanged("Connected");
            }
        }

        private bool _scanStatus;
        public bool ScanStatus
        {
            get { return _scanStatus; }
            set
            {
                _scanStatus = value;
                OnPropertyChanged("ScanStatus");
            }
        }

        public bool IsScanning { get { return _client.IsScanning(); } }

        public BluetoothViewModel(IBluetoothClient c)
        {
            _client = c;
            _client.DeviceDiscovered += _client_DeviceDiscovered;
            _client.DeviceConnected += _client_DeviceConnected;
            _client.DeviceConnectionLost += _client_DeviceConnectionLost;
            _client.DeviceDisconnected += _client_DeviceDisconnected;
            _client.ScanTimeoutElapsed += _client_ScanTimeoutElapsed;
        }

        #region Events
        private void _client_DeviceDisconnected(IDevice device)
        {
            ConnectionStatus = ConnectionState.Disconnected;
            // ConnectionStatus = string.Format("Device {0} disconnected.", device.Name);
            Connected = false;
        }

        private void _client_DeviceConnectionLost(IDevice device, string errorMessage)
        {
            ConnectionStatus = ConnectionState.ConnectionLost;
            // ConnectionStatus = string.Format("Lost connection with device {0}.{1}Errormessage: {2}", device.Name, System.Environment.NewLine, errorMessage);
            Connected = false;
        }

        private void _client_DeviceConnected(IDevice device)
        {
            ConnectedDevice = device;
            ConnectionStatus = ConnectionState.Connected;
            Connected = true;
        }

        private void _client_DeviceDiscovered(IDevice device)
        {
            if (!string.IsNullOrEmpty(device.Name))
            {
                if (device.Name.Equals("<*_*>"))
                {
                    if (!FoundDevices.Contains(device))
                    {
                        FoundDevices.Add(device);
                    }
                }
            }
        }

        private void _client_ScanTimeoutElapsed()
        {
            ScanStatus = false;
        }
        #endregion

        #region Scanning stuff

        /// <summary>
        /// Stops the scan.
        /// </summary>
        /// <returns></returns>
        public async void StopScanningForDevicesAsync()
        {
            ScanStatus = false;
            await _client.StopScanForDevicesAsync();
        }

        /// <summary>
        /// Starts a scan for LE Devices.
        /// </summary>
        /// <returns></returns>
        public async void StartScanningForDevicesAsync()
        {
            ScanStatus = true;
            await _client.StartScanForDevicesAsync();
        }
        #endregion

        #region Connection stuff
        /// <summary>
        /// Get the paired devices and store them in PairedDevices.
        /// </summary>
        public void GetPairedOrKnownDevices()
        {
            PairedDevices = _client.GetPairedDevices();
        }

        /// <summary>
        /// Connect to device async.
        /// </summary>
        /// <param name="device">IDevice of the target</param>
        public async Task ConnectToDeviceAsync(IDevice device)
        {
            // ToDo: Failure handling
            await _client.ConnectToDeviceAsync(device);
            if (await SetServiceFromConnectedDeviceByName())
            {
                if (await SetCharacteristicFromServiceByName()) { }
            }
        }

        /// <summary>
        /// Connect to known device async.
        /// </summary>
        /// <param name="id">Guid of the target</param>
        public async Task ConnectToKnownDeviceAsync(Guid id)
        {
            await _client.ConnectToKnownDeviceAsync(id);
        }

        /// <summary>
        /// Disconnect device async.
        /// </summary>
        /// <param name="device">IDevice of the target</param>
        public async Task DisconnectDeviceAsync(IDevice device)
        {
            await _client.DisconnectDeviceAsync(device);
        }
        #endregion

        #region Services and characteristics
        /// <summary>
        /// Set the service if a service is found by name.
        /// </summary>
        /// <param name="name">Name of the service</param>
        public async Task<bool> SetServiceFromConnectedDeviceByName()
        {
            if (await _client.SetServiceByNameAsync(NAME_OF_SERVICE)) return true;
            return false;
        }

        /// <summary>
        /// Set the characteristic from service if it is found by name.
        /// </summary>
        /// <param name="name">Name of the characteristic</param>
        public async Task<bool> SetCharacteristicFromServiceByName()
        {
            if (await _client.SetCharacteristicByNameAsync(NAME_OF_CHARACTERISTIC)) return true;
            return false;
        }
        #endregion

        #region Sending and receiving
        public async Task<byte[]> ReadAsync()
        {
            return await _client.ReadAsync();
        }

        public async Task WriteAsync(byte[] data)
        {
            await _client.WriteAsync(data);
        }
        #endregion

        /// <summary>
        /// #1: Disconnected, #2: Connected, #3: ConnectionLost
        /// </summary>
        public enum ConnectionState
        {
            Disconnected = 1,
            Connected = 2,
            ConnectionLost = 3
        }
    }
}
