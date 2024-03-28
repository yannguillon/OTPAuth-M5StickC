const TerserPlugin = require("terser-webpack-plugin");
const path = require('path');
const isProduction = process.env.NODE_ENV == 'production';

const config = {
    entry: {
        index: {
            import: [
                './webserver/lib/javascript/lib/browserified-jscrypto.js',
                './webserver/lib/javascript/lib/browserified-buffer.js',
                './webserver/lib/javascript/lib/browserified-pblist-parser.js',
                './webserver/lib/javascript/lib/flowbite.js',
                './webserver/lib/javascript/crypto.js',
                './webserver/lib/javascript/otp.js',
                './webserver/lib/javascript/base.js'
            ]
        }
    },
    optimization: {
        minimize: true,
        minimizer: [
            new TerserPlugin({
                extractComments: false,
            }),
        ],
    },
    output: {
        path: path.resolve(__dirname, 'webserver', 'public'),
    },
    plugins: [
        // Add your plugins here
        // Learn more about plugins from https://webpack.js.org/configuration/plugins/
    ],
    module: {
        rules: [
            {
                test: /\.(js|jsx)$/i,
                loader: 'babel-loader',
            }
        ],
    },
};

module.exports = () => {
    if (isProduction) {
        config.mode = 'production';


    } else {
        config.mode = 'development';
    }
    return config;
};
