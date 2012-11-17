#include <stdio.h>
#include <stdlib.h>
#include <libfprint/fprint.h>
#include <malloc.h>

void FpInit()
{
        if(fp_init() < 0) {
                perror("initialize failed:");
                exit(1);

        }
        printf("initialize successed\n");

}

struct fp_dev *FpOpen(struct fp_dscv_dev *dev)
{
        struct fp_dev *fDev;
        fDev = fp_dev_open(dev);
        if(fDev == NULL){
                perror("can't open fp_dev");
                exit(1);
        } 
        printf("open fp_dev successed!\n");
        return fDev; 
}

struct fp_dev *DiscoverDevs()
{
        struct fp_dscv_dev **devs;
        struct fp_dscv_dev *dev;
        struct fp_driver *drv;
        struct fp_dev *fDev;

        devs = fp_discover_devs();
        if(!devs) {
                perror("can't dicover devices:");
                exit(1);  
        } 
        dev = devs[0];
        if(!dev) {
                perror("no device:");
                exit(1);         
        }
        drv = fp_dscv_dev_get_driver(dev);
        printf("device name:%s\n",fp_driver_get_full_name(drv));
        fDev = FpOpen(dev);
        fp_dscv_devs_free(devs);
        return fDev;       
}

struct fp_print_data *enroll(struct fp_dev *fDev)
{
        struct fp_print_data *print_data;
        struct fp_img *img;
        int flag;
        printf("enroll stages are needed %d\n", 
                        fp_dev_get_nr_enroll_stages(fDev));
        flag = fp_enroll_finger_img(fDev, &print_data, &img); 
        if(img) {
                printf("height:%d\nwidth:%d",
                                fp_img_get_height(img),
                                fp_img_get_width(img));
                fp_img_save_to_file(img, "test.pgm");
                printf("write in test.pgm\n"); 
        }
        fp_img_free(img);
}

void main()
{
        struct fp_dev *fDev;
        struct fp_print_data *print_data;
        FpInit();
        fDev = DiscoverDevs();
        print_data = enroll(fDev);
        fp_dev_close(fDev);
}
