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
        private IList<IService> _services;
        public IList<IService> Services
        {
            get { return _services; }
            set
            {
                if (_services != value)
                {
                    _services = value;
                    OnPropertyChanged("Services");
                }
            }
        }
        private IEnumerable<ICharacteristic> _characteristics;
        public IEnumerable<ICharacteristic> Characteristics
        {
            get { return _characteristics; }
            set
            {
                if (_characteristics != value)
                {
                    _characteristics = value;
                    OnPropertyChanged("Characteristics");
                }
            }
        }

        private string _connectionStatus;
        public string ConnectionStatus
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

        public bool IsScanning { get { return _client.IsScanning(); } }

        public BluetoothViewModel(IBluetoothClient c)
        {
            _client = c;
            _client.DeviceDiscovered += _client_DeviceDiscovered;
            _client.DeviceConnected += _client_DeviceConnected;
            _client.DeviceConnectionLost += _client_DeviceConnectionLost;
            _client.DeviceDisconnected += _client_DeviceDisconnected;

        }
        #region Events
        private void _client_DeviceDisconnected(IDevice device)
        {
            ConnectionStatus = string.Format("Device {0} disconnected.", device.Name);
        }

        private void _client_DeviceConnectionLost(IDevice device, string errorMessage)
        {
            ConnectionStatus = string.Format("Lost connection with device {0}.{1}Errormessage: {2}", device.Name, System.Environment.NewLine, errorMessage);
        }

        private void _client_DeviceConnected(IDevice device)
        {
            ConnectionStatus = string.Format("Succesfully connected to device {0}.", device.Name);
        }

        private void _client_DeviceDiscovered(IDevice device)
        {
            FoundDevices.Add(device);
        }
        #endregion

        #region Scanning stuff

        /// <summary>
        /// Stops the scan.
        /// </summary>
        /// <returns></returns>
        public async void StopScanningForDevicesAsync()
        {
            await _client.StopScanForDevices();
        }

        /// <summary>
        /// Starts a scan for LE Devices.
        /// </summary>
        /// <returns></returns>
        public async void StartScanningForDevicesAsync()
        {
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
        public async void ConnectToDeviceAsync(IDevice device)
        {
            await _client.ConnectToDeviceAsync(device);
        }

        /// <summary>
        /// Connect to known device async.
        /// </summary>
        /// <param name="id">Guid of the target</param>
        public async void ConnectToKnownDeviceAsync(Guid id)
        {
            await _client.ConnectToKnownDeviceAsync(id);
        }

        /// <summary>
        /// Disconnect device async.
        /// </summary>
        /// <param name="device">IDevice of the target</param>
        public async void DisconnectDeviceAsync(IDevice device)
        {
            await _client.DisconnectDeviceAsync(device);
        }
        #endregion

        #region Services and characteristics
        /// <summary>
        /// Get the services from connected device and store in Services.
        /// </summary>
        /// <param name="positionInList">Position of the target in ConnectedDevices list</param>
        public async void UpdateServicesFromConnectedDevice(int positionInList)
        {
            Services = await _client.UpdateServicesAsync(positionInList);
        }

        /// <summary>
        /// Get the characteristics from service.
        /// </summary>
        /// <param name="positionInConnectedDevices">Position of the target in ConnectedDevices list</param>
        /// <param name="positionInServices">Position of the target in Services list</param>
        public async void UpdateCharacteristicsFromService(int positionInConnectedDevices, int positionInServices)
        {
            Characteristics = await _client.UpdateCharacteristicsAsync(positionInServices);
        }
        #endregion

        #region Sending and receiving
        public async Task<byte[]> ReadAsync()
        {
            return await _client.ReadAsync();
        }

        public async void WriteAsync(byte[] data)
        {
            // Todo
        }
        #endregion
    }
}
