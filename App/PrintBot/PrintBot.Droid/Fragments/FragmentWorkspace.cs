using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Util;
using Android.Views;
using Android.Widget;
using System.Collections.ObjectModel;
using PrintBot.Droid.Controls;
using PrintBot.Droid.Controls.Blocks;
using PrintBot.Domain.Models.Blocks;
using PrintBot.Infrastructure.Services;
using PrintBot.Infrastructure.ViewModels;
using PrintBot.Droid.Activities;

namespace PrintBot.Droid
{
    public class FragmentWorkspace : Fragment
    {
        CodeEditor_BaseActivity _activity;
        public ObservableCollection<BlockListItem> List
        {
            get
            {
                return _activity.List;
            }
        }

        DraggableListView listView;
        DraggableListAdapter adapter;
        public override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            _activity = (CodeEditor_BaseActivity)Activity;
            //List = new ObservableCollection<BlockListItem>();
            adapter = new DraggableListAdapter(Context, List);
        }

        public override View OnCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
        {
            var view = inflater.Inflate(Resource.Layout.fragment_workspace, container, false);
            listView = view.FindViewById<DraggableListView>(Resource.Id.workspace_listView);
            listView.Adapter = adapter;
            listView.ReorderingEnabled = true;
            listView.Drag += HandleDrag;

            return view;
        }

        void HandleDrag(object sender, View.DragEventArgs e)
        {
            var evt = e.Event;

            switch (evt.Action)
            {
                case DragAction.Started:
                    /* To register your view as a potential drop zone for the current view being dragged
                     * you need to set the event as handled
                     */
                    e.Handled = true;

                    /* An important thing to know is that drop zones need to be visible (i.e. their Visibility)
                     * property set to something other than Gone or Invisible) in order to be considered. A nice workaround
                     * if you need them hidden initially is to have their layout_height set to 1.
                     */

                    break;
                case DragAction.Entered:

                case DragAction.Exited:
                    /* These two states allows you to know when the dragged view is contained atop your drop zone.
                     * Traditionally you will use that tip to display a focus ring or any other similar mechanism
                     * to advertise your view as a drop zone to the user.
                     */

                    break;
                case DragAction.Drop:
                    /* This state is used when the user drops the view on your drop zone. If you want to accept the drop,
                     *  set the Handled value to true like before.
                     */
                    e.Handled = true;
                    /* It's also probably time to get a bit of the data associated with the drag to know what
                     * you want to do with the information.
                     */
                    ListView lv = (ListView)sender;
                    var block = (BlockListItem)e.Event.LocalState;

                    // Get the Instance for the List
                    var tmpBlock = block.GetAnInstanceAndInitialize();

                    // Get the Position in the List
                    var position = lv.PointToPosition((int)e.Event.GetX(), (int)e.Event.GetY());
                    position = position == -1 ? List.Count : position;
                    List.Insert(position, tmpBlock);

                    if (tmpBlock.BlockType == BlockListItem.BlockTypeEnum.IfBlock)
                    {
                        position++;
                        List.Insert(position, block.GetAnInstanceOfElseBlock());
                    }
                    if (tmpBlock.BlockHolder.Block is StartBlock)
                    {
                        position++;
                        List.Insert(position, block.GetAnInstanceOfEndBlock(block.BlockType));
                    }

                    break;
                case DragAction.Ended:
                    /* This is the final state, where you still have possibility to cancel the drop happened.
                     * You will generally want to set Handled to true.
                     */
                    e.Handled = true;
                    break;
            }
        }
    }
}