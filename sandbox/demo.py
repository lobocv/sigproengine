from np_opencv_module import np_opencv_module

from pylib import sensor_network_py as SN
from pylib import empath5_py as empath5
from pylib import radarview_py as View
from cv2helpers import *

import numpy as np
from math import ceil
import time
import cv2
import os
import glob

bag_files = glob.glob("../../bagfiles/*.bag")

if len(bag_files) > 1:
    print "Select a bagfile:"
    for ii, bag in enumerate(bag_files):
        print "%d. %s" % (ii+1, bag)

    while 1:
        try:
            bag_num = int(raw_input())
        except Exception as e:
            print e
        else:
            break
else:
    bag_num = 1

if bag_num == KEY_ESC:
    exit(0)
else:
    try:
        bag_path = bag_files[bag_num-1]
    except IndexError:
        print "Invalid bag file selected, using the first one in the list."
        bag_path = bag_files[0]

topic = '/dt1'
data = SN.getRadarBuffer(bag_path, topic)

points_per_trace, number_of_traces = data.shape
tsi_ns = 0.4
tsi_ps = tsi_ns * 1000
step_size_m = .05
frequency_MHz = 250.


cs = View.CrossSection()
cs.timeAxis.minAxisValue = 0
cs.timeAxis.maxAxisValue = points_per_trace * tsi_ns
cs.timeAxis.major_tick_dist = 10
cs.timeAxis.n_minor_ticks = 4

cs.positionAxis.minAxisValue = 0
cs.positionAxis.maxAxisValue = (number_of_traces-1) * step_size_m
cs.positionAxis.major_tick_dist = 2
cs.positionAxis.n_minor_ticks = 4


scope = View.TracePlot()
scope.setPadding_px(100, 50, 50, 50)
scope.orientation = View.Orientation.horizontal
scope.plot_width = 800
scope.plot_height = 200

scope.timeAxis.minAxisValue = 0
scope.timeAxis.maxAxisValue = points_per_trace * tsi_ns
scope.timeAxis.major_tick_dist = 10
scope.timeAxis.n_minor_ticks = 4

scope.amplitudeAxis.minAxisValue = -50.
scope.amplitudeAxis.maxAxisValue = 50.
scope.amplitudeAxis.major_tick_dist = 10
scope.amplitudeAxis.n_minor_ticks = 4





def dc_removal():


    while 1:
        try:
            dc_points = int(raw_input("Enter a Number of DC points to average:"))
        except Exception as e:
            print e
        else:
            break

    print "Press ENTER to toggle DC Removal on or off, ESCAPE to exit."

    do_dc_removal = False
    while 1:
        traces = data.copy(order='F')

        if do_dc_removal:
            empath5.SimpleDCOffsetRemoval(traces, traces, dc_points)

        mat = cs.arrayToMatrix(traces, step_size_m, tsi_ns)

        cv2.imshow(bag_path, mat)
        print "DC Removal: %s" % ("ON" if do_dc_removal else "OFF")
        key = awaitKeys([KEY_ENTER, KEY_ESC], 0)
        if key == KEY_ENTER:
            do_dc_removal = not do_dc_removal
        elif key == KEY_ESC:
            return


def interpolate_trace():
    pt0 = 0
    pt1 = data.shape[0]
    points_per_trace = pt1 - pt0

    n_crossings = 10; threshold_mV=3; start_search_idx=20;

    scope.timeAxis.minAxisValue = pt0 * tsi_ns
    scope.timeAxis.maxAxisValue = pt1 * tsi_ns

    interp_points = int(raw_input("Enter a number of points to interpolate between each original point. ESCAPE to exit."))
    print "Interpolating each point by %d points" % interp_points

    interp_method_key = int(raw_input("Enter an interpolation method. ESCAPE to exit.\n linear = 1\n yves_spline = 2\n"))
    interp_method = {1: "linear", 2: "yves_spline"}[interp_method_key]

    for trace_idx in xrange(number_of_traces):
        trace = data[pt0:pt1, trace_idx:trace_idx+1].copy(order='F')

        new_ppt = empath5.get_interpolated_length(points_per_trace, interp_points)
        new_tsi_ns = tsi_ns / (interp_points+1)

        interp_trace = np.zeros((new_ppt, 1), dtype=trace.dtype)
        empath5.InterpolateTrace(trace, interp_trace, interp_points, interp_method)

        scope.timeAxis.maxAxisValue = new_ppt * new_tsi_ns

        zeroes_indices = np.ones(n_crossings, dtype=np.int32)
        empath5.FindZeroCrossings(interp_trace, zeroes_indices, n_crossings, threshold_mV, empath5.get_interpolated_length(start_search_idx, interp_points))
        zeroes_indices = zeroes_indices.astype(float)

        mat = scope.render(interp_trace, new_tsi_ns, zeroes_indices)
        cv2.imshow(bag_path, mat)
        cv2.waitKey(20)



def scroll_display():

    for ii in xrange(1000):
        traces = data.copy(order='F')

        # Shift traces (for fun). Not sure why np.roll() does not work as expected.
        front = traces[:, :ii]
        end = traces[:, ii:]
        traces = np.hstack((end, front))

        mat = cs.render(traces, step_size_m, tsi_ns)
        cv2.imshow(bag_path, mat)
        cv2.waitKey(0 if ii == 0 else 1)
        time.sleep(0.01)


def find_first_break():
    print "Press ENTER to toggle First break Line on or off, ESCAPE to exit."

    traces = data.copy(order='F')
    fb_indices = np.ones(number_of_traces)

    while 1:
        try:
            threshold_mV = float(raw_input("Enter a threshold value to pick first break from (mV):"))
        except Exception as e:
            print e
        else:
            break

    empath5.FindFirstBreakIndex(traces, fb_indices, threshold_mV)

    for ii, fb in enumerate(fb_indices):
        print "Trace Number %d has a first break index of %f" % (ii+1, fb)



def show_trace():

    trace = data[:, 0].copy(order='F')
    mat = scope.render(trace, tsi_ns)
    cv2.imshow(bag_path, mat)

    while 1:
        print "Enter a trace number to plot:"
        trace_number = awaitNumber()
        if trace_number == KEY_ESC:
            break

        trace = data[:, int(trace_number-1)].copy(order='F')

        mat = scope.render(trace, tsi_ns)

        cv2.imshow(bag_path, mat)


def find_zero_crossings():
    print "Press ENTER to toggle First break Line on or off, ESCAPE to exit."

    traces = data.copy(order='F')
    print traces.shape
    while 1:
        try:
            n_crossings = int(raw_input("Enter the number of zero crossings to find (default 4):"))
            threshold_mV = float(raw_input("Enter the threshold between zeros (mV):"))
            start_search_idx = int(raw_input("Enter the starting search index:"))
        except Exception as e:
            print e
        else:
            break

    zeroes_indices = np.ones(n_crossings, dtype=np.int32)

    empath5.FindZeroCrossings(traces[:,0], zeroes_indices, n_crossings, threshold_mV, start_search_idx)

    print "Zero crossings are at indices:"
    for ii, zc in enumerate(zeroes_indices):
        print zc, traces[zc-1, 0], traces[zc, 0], traces[zc+1, 0]



app_map = { 1: scroll_display,
            2: dc_removal,
            3: show_trace,
            4: interpolate_trace,
            5: find_first_break,
            6: find_zero_crossings,
            }

app_number = raw_input("Select the app to run: \n%s\n" % '\n'.join(["%d. %s" % (i, func.__name__ ) for i, func in app_map.iteritems()]))

try:
    app_map[int(app_number)]()
except ValueError:
    pass

