import cv2

#cap = cv2.VideoCapture()
#cap2 = cv2.VideoCapture(2)
cap = cv2.VideoCapture("/dev/v4l/by-id/usb-e-con_systems_See3CAM_20CUG_0E0B9504-video-index0",cv2.CAP_V4L2)
cap2 =  cv2.VideoCapture("/dev/v4l/by-id/usb-e-con_systems_See3CAM_20CUG_2B199904-video-index0",cv2.CAP_V4L2)


cap.set(cv2.CAP_PROP_CONVERT_RGB, 0)
cap.set(cv2.CAP_PROP_EXPOSURE,100)
cap2.set(cv2.CAP_PROP_CONVERT_RGB, 0)
cap2.set(cv2.CAP_PROP_EXPOSURE,100)

num = 0

while cap.isOpened():

    succes1, img = cap.read()
    succes2, img2 = cap2.read()
    img=cv2.convertScaleAbs(img, 0.2490234375)
    img2=cv2.convertScaleAbs(img2, 0.2490234375)
    k = cv2.waitKey(5)

    if k == 27:
        break
    elif k == ord('s'): # wait for 's' key to save and exit
        cv2.imwrite('images/stereoLeft/imageL' + str(num) + '.png', img)
        cv2.imwrite('images/stereoRight/imageR' + str(num) + '.png', img2)
        print("images saved!")
        num += 1

    cv2.imshow('Img 1',img)
    cv2.imshow('Img 2',img2)

# Release and destroy all windows before termination
cap.release()
cap2.release()

cv2.destroyAllWindows()
