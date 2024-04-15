const TerserPlugin = require("terser-webpack-plugin");
const path = require('path');
const isProduction = process.env.NODE_ENV == 'production';

const config = {
    entry: {
        app: { import: './webserver/javascript/app/app.js' }
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
    module: {
        rules: [
            {
                test: /\.(js|jsx)$/i,
                loader: 'babel-loader'
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
