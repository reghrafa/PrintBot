using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;

namespace PrintBot.Android
{
    //The service locator creates and manages instances of ViewModels and Services
    public class ServiceLocator
    {
        #region Singleton
        //Singleton implementation -> There is always just one instance that keeps the references to the VM and service instances
        private static ServiceLocator _current;

        public static ServiceLocator Current
        {
            get { return _current ?? (_current = new ServiceLocator()); }
        }

        public static void Reset()
        {
            _current = null;
        }

        #endregion

        /// <summary>
        /// Constructor
        /// </summary>
        private ServiceLocator()
        {
        }
    }
}