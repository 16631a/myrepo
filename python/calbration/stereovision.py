import numpy as np
import cv2
#############################
def update_map(ind, map_x, map_y):
    if ind == 0:
        for i in range(map_x.shape[0]):
            for j in range(map_x.shape[1]):
                if j > map_x.shape[1]*0.25 and j < map_x.shape[1]*0.75 and i > map_x.shape[0]*0.25 and i < map_x.shape[0]*0.75:
                    map_x[i,j] = 2 * (j-map_x.shape[1]*0.25) + 0.5
                    map_y[i,j] = 2 * (i-map_y.shape[0]*0.25) + 0.5
                else:
                    map_x[i,j] = 0
                    map_y[i,j] = 0
    elif ind == 1:
        for i in range(map_x.shape[0]):
            map_x[i,:] = [x for x in range(map_x.shape[1])]
        for j in range(map_y.shape[1]):
            map_y[:,j] = [map_y.shape[0]-y for y in range(map_y.shape[0])]
    elif ind == 2:
        for i in range(map_x.shape[0]):
            map_x[i,:] = [map_x.shape[1]-x for x in range(map_x.shape[1])]
        for j in range(map_y.shape[1]):
            map_y[:,j] = [y for y in range(map_y.shape[0])]
    elif ind == 3:
        for i in range(map_x.shape[0]):
            map_x[i,:] = [map_x.shape[1]-x for x in range(map_x.shape[1])]
        for j in range(map_y.shape[1]):
            map_y[:,j] = [map_y.shape[0]-y for y in range(map_y.shape[0])]
#############################33

# Camera parameters to undistort and rectify images
cv_file = cv2.FileStorage()
cv_file.open('stereoMap.xml', cv2.FileStorage_READ)

stereoMapL_x = cv_file.getNode('stereoMapL_x').mat()
stereoMapL_y = cv_file.getNode('stereoMapL_y').mat()
stereoMapR_x = cv_file.getNode('stereoMapR_x').mat()
stereoMapR_y = cv_file.getNode('stereoMapR_y').mat()


# Open both cameras
cap_right = cv2.VideoCapture("/dev/v4l/by-id/usb-e-con_systems_See3CAM_20CUG_0E0B9504-video-index0")                    
cap_left =  cv2.VideoCapture("/dev/v4l/by-id/usb-e-con_systems_See3CAM_20CUG_2B199904-video-index0")
cap_left.set(cv2.CAP_PROP_CONVERT_RGB, 0)
cap_left.set(cv2.CAP_PROP_EXPOSURE,100)
cap_right.set(cv2.CAP_PROP_CONVERT_RGB, 0)
cap_right.set(cv2.CAP_PROP_EXPOSURE,100)

print ("hello")
while(cap_right.isOpened() and cap_left.isOpened()):
    succes_right, frame_right = cap_right.read()
    succes_left, frame_left = cap_left.read()
    frame_right=cv2.convertScaleAbs(frame_right, 0.2490234375)
    frame_left=cv2.convertScaleAbs(frame_left, 0.2490234375)
    cv2.imshow("frame rr right", frame_right)
    cv2.imshow("framerr left", frame_left)
    #update_map(0, stereoMapR_x, stereoMapR_y)
    # Undistort and rectify images
    #frame_right = cv2.remap(frame_right, stereoMapR_x, stereoMapR_y, cv2.INTER_LANCZOS4, cv2.BORDER_CONSTANT, 0)
    frame_right = cv2.remap(frame_right, stereoMapR_x, stereoMapR_y,  cv2.INTER_LINEAR)
    frame_left = cv2.remap(frame_left, stereoMapL_x, stereoMapL_y, cv2.INTER_LANCZOS4, cv2.BORDER_CONSTANT, 0)
                     
    # Show the frames
    cv2.imshow("frame right", frame_right) 
    cv2.imshow("frame left", frame_left)


    # Hit "q" to close the window
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break


# Release and destroy all windows before termination
cap_right.release()
cap_left.release()

cv2.destroyAllWindows()
