#include <Python.h>
#include "py3.h"
#include <webp/encode.h>
#include <webp/decode.h>
#include <webp/types.h>


PyObject* WebPGetFeatures_wrapper(PyObject* self, PyObject* args)
{
    PyBytesObject *webp_string;
    const uint8_t* webp = NULL;
    VP8StatusCode vp8_status_code = VP8_STATUS_OK;
    Py_ssize_t size;
    WebPBitstreamFeatures* const features;


    if (!PyArg_ParseTuple(args, "S", &webp_string)) {
        Py_INCREF(Py_None);
        return Py_None;
    }

    PyBytes_AsStringAndSize((PyObject *) webp_string, (char**)&webp, &size);

    vp8_status_code = WebPGetFeatures(webp, size, features);

    if (vp8_status_code == VP8_STATUS_OK) {
        printf("%i", features->has_alpha);

    } else {
        // TODO: raise some sort of error
        printf("Error occured checking webp file with code: %d\n", vp8_status_code);
        Py_INCREF(Py_None);
        return Py_None;
    }

    free((void*)webp);
    return Py_BuildValue("b", features->has_alpha);
}


PyObject* WebPEncodeRGB_wrapper(PyObject* self, PyObject* args)
{
    PyBytesObject *rgb_string;
    int width;
    int height;
    int stride;
    float quality_factor;
    uint8_t *rgb;
    uint8_t *output;
    Py_ssize_t size;
    size_t ret_size;

    if (!PyArg_ParseTuple(args, "Siiif", &rgb_string, &width, &height, &stride, &quality_factor)) {
        Py_INCREF(Py_None);
        return Py_None;
    }

    PyBytes_AsStringAndSize((PyObject *) rgb_string, (char**)&rgb, &size);

    if (stride * height > size) {
        Py_INCREF(Py_None);
        return Py_None;
    }

    ret_size = WebPEncodeRGB(rgb, width, height, stride, quality_factor, &output);
    if (ret_size > 0) {
        PyObject *ret = PyBytes_FromStringAndSize((char*)output, ret_size);
        free(output);
        return ret;
    }
    Py_INCREF(Py_None);
    return Py_None;

}


PyObject* WebPEncodeRGBA_wrapper(PyObject* self, PyObject* args)
{
    PyBytesObject *rgba_string;
    int width;
    int height;
    int stride;
    float quality_factor;
    uint8_t *rgba;
    uint8_t *output;
    Py_ssize_t size;
    size_t ret_size;

    if (!PyArg_ParseTuple(args, "Siiif", &rgba_string, &width, &height, &stride, &quality_factor)) {
        Py_INCREF(Py_None);
        return Py_None;
    }

    PyBytes_AsStringAndSize((PyObject *) rgba_string, (char**)&rgba, &size);

    if (stride * height > size) {
        Py_INCREF(Py_None);
        return Py_None;
    }

    ret_size = WebPEncodeRGBA(rgba, width, height, stride, quality_factor, &output);
    if (ret_size > 0) {
        PyObject *ret = PyBytes_FromStringAndSize((char*)output, ret_size);
        free(output);
        return ret;
    }
    Py_INCREF(Py_None);
    return Py_None;

}


PyObject* WebPDecodeRGB_wrapper(PyObject* self, PyObject* args)
{
    PyBytesObject *webp_string;
    int width;
    int height;
    uint8_t *webp;
    uint8_t *output;
    Py_ssize_t size;
    PyObject *ret;

    if (!PyArg_ParseTuple(args, "S", &webp_string)) {
        Py_INCREF(Py_None);
        return Py_None;
    }

    PyBytes_AsStringAndSize((PyObject *) webp_string, (char**)&webp, &size);

    output = WebPDecodeRGB(webp, size, &width, &height);

    ret = PyBytes_FromStringAndSize((char*)output, width * height * 3);
    free(output);
    return Py_BuildValue("Sii", ret, width, height);
}


PyObject* WebPDecodeRGBA_wrapper(PyObject* self, PyObject* args)
{
    PyBytesObject *webp_string;
    int width;
    int height;
    uint8_t *webp;
    uint8_t *output;
    Py_ssize_t size;
    PyObject *ret;

    if (!PyArg_ParseTuple(args, "S", &webp_string)) {
        Py_INCREF(Py_None);
        return Py_None;
    }

    PyBytes_AsStringAndSize((PyObject *) webp_string, (char**)&webp, &size);

    output = WebPDecodeRGBA(webp, size, &width, &height);

    ret = PyBytes_FromStringAndSize((char*)output, width * height * 4);
    free(output);
    return Py_BuildValue("Sii", ret, width, height);
}


static PyMethodDef webpMethods[] =
{
    {"WebPGetFeatures", WebPGetFeatures_wrapper, METH_VARARGS, "WebPGetFeatures"},
    {"WebPEncodeRGB", WebPEncodeRGB_wrapper, METH_VARARGS, "WebPEncodeRGB"},
    {"WebPEncodeRGBA", WebPEncodeRGBA_wrapper, METH_VARARGS, "WebPEncodeRGBA"},
    {"WebPDecodeRGB", WebPDecodeRGB_wrapper, METH_VARARGS, "WebPDecodeRGB"},
    {"WebPDecodeRGBA", WebPDecodeRGBA_wrapper, METH_VARARGS, "WebPDecodeRGBA"},
    {NULL, NULL}
};


#if PY_VERSION_HEX >= 0x03000000
PyMODINIT_FUNC
PyInit__webp(void) {
    PyObject* m;

    static PyModuleDef module_def = {
        PyModuleDef_HEAD_INIT,
        "_webp",            /* m_name */
        NULL,               /* m_doc */
        -1,                 /* m_size */
        webpMethods,        /* m_methods */
    };

    m = PyModule_Create(&module_def);
    return m;
}
#else
PyMODINIT_FUNC
init_webp(void)
{
    Py_InitModule("_webp", webpMethods);
}
#endif
