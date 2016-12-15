using Plugin.BLE.Abstractions.Contracts;
using PrintBot.Domain.Models;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PrintBot.Domain.RequiredInterfaces
{
    public delegate void DeviceConnectedEventHandler(IDevice device);
    public delegate void DeviceDiscoveredEventHandler(IDevice device);
    public delegate void DeviceConnectionLostEventHandler(IDevice device, string errorMessage);
    public delegate void DeviceDisconnectedEventHandler(IDevice device);
    public delegate void ScanTimeoutElapsedEventHandler();

    public interface IBluetoothClient
    {
        // Adapter events
        event DeviceDiscoveredEventHandler DeviceDiscovered;
        event DeviceConnectedEventHandler DeviceConnected;
        event DeviceConnectionLostEventHandler DeviceConnectionLost;
        event DeviceDisconnectedEventHandler DeviceDisconnected;
        event ScanTimeoutElapsedEventHandler ScanTimeoutElapsed;

        bool IsScanning();

        void SetScanTimeout(int ms);
        int GetScanTimeout();
        Task StartScanForDevicesAsync();
        Task StopScanForDevicesAsync();
        Task ConnectToDeviceAsync(IDevice device);
        Task ConnectToKnownDeviceAsync(Guid id);
        Task DisconnectDeviceAsync(IDevice device);
        ObservableCollection<IDevice> GetPairedDevices();
        Task<bool> IsValidDeviceAsync(string serviceName, string characteristicName, IDevice device);
        Task<bool> SetServiceByNameAsync(string name);
        Task<bool> SetCharacteristicByNameAsync(string name);
        Task<byte[]> ReadAsync();
        Task WriteAsync(byte[] data);
    }
}
